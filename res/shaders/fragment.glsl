#version 450 core

struct Camera {
    vec3 position;
};

struct DirectionalLight {
    vec3 color;
    vec3 direction;
    float intensity;
    sampler2DShadow shadow_sampler;
    mat4 vp;
};

struct PointLight {
    vec3 color;
    vec3 position;
    float intensity;
};

struct SpotLight {
    vec3 color;
    vec3 position;
    vec3 direction;
    float angle;
    float intensity;
    sampler2DShadow shadow_sampler;
    mat4 vp;
    float near;
    float far;
};

struct XrayLight {
    vec3 position;
    vec3 direction;
    float angle;
    sampler2DShadow shadow_sampler;
    mat4 vp;
    float near;
    float far;
};

struct Component {
    bool textured;
    sampler2D sampler;
    vec3 color;
};

struct Material {
    Component ambient;
    Component diffuse;
    Component specular;
    float shininess;
    bool normal_textured;
    sampler2D normal_sampler;
};

// Constants
const float AMBIENT_STRENGTH = 0.15;

const float PI = 3.14159;

const vec2 POISSON_DISK[4] = vec2[](
    vec2(-0.94201624, -0.39906216),
    vec2(0.94558609, -0.76890725),
    vec2(-0.094184101, -0.92938870),
    vec2(0.34495938, 0.29387760)
);

// Uniforms
uniform Camera camera;
uniform DirectionalLight directional_light;
uniform PointLight point_light;
uniform SpotLight spot_light;
uniform XrayLight xray_light;
uniform Material material;
uniform bool xrayable;
uniform bool toon_enabled;
uniform sampler2D toon_map;

// Inputs
layout(location = 0) in vec3 fragment_position;
layout(location = 1) in vec3 fragment_normal;
layout(location = 2) in vec2 fragment_texture_coord;

// Outputs
layout(location = 0) out vec3 out_color;

float linearize_depth(float depth, float near, float far) {
    float z = 2.0 * depth - 1.0;
    return 2.0 * near * far / (far + near - z * (far - near));
}

float delinearize_depth(float depth, float near, float far) {
    float z = -((2.0 * near * far / depth) - far - near) / (far - near);
    return (z + 1.0) * 0.5;
}

float compute_bias_normalized(vec3 normal, vec3 light_direction, sampler2DShadow sampler, float depth) {
    float light_angle = acos(max(0.0, dot(normal, light_direction)));
    float sqrt2 = 1.414;
    float half_pixel_size = sqrt2 / textureSize(sampler, 0).x;
    return half_pixel_size * tan(light_angle);
}

float compute_bias(vec3 normal, vec3 light_direction, sampler2DShadow sampler, float depth,  float near, float far) {
    float normalized_bias = compute_bias_normalized(normal, light_direction, sampler, depth);
    float linearized_depth = linearize_depth(depth, near, far);
    float epsilon = 0.001 * linearized_depth;
    float optimal_bias = normalized_bias * linearized_depth;

    float new_depth = linearized_depth - (epsilon + optimal_bias);
    float delinearized_new_depth = delinearize_depth(new_depth, near, far);
    return abs(depth - delinearized_new_depth);
}

vec3 compute_normal() {

    vec3 normal = normalize(fragment_normal);
    if (!material.normal_textured) {
        return normal;
    }

    // derivations of the fragment position
    vec3 pos_dx = dFdx(fragment_position);
    vec3 pos_dy = dFdy(fragment_position);

    // derivations of the texture coordinate
    vec2 texC_dx = dFdx(fragment_texture_coord);
    vec2 texC_dy = dFdy(fragment_texture_coord);

    // tangent vector and binormal vector
    vec3 tangent = normalize(texC_dy.y * pos_dx - texC_dx.y * pos_dy);
    vec3 bitangent = normalize(texC_dx.x * pos_dy - texC_dy.x * pos_dx);

    vec4 normal_map_value = texture(material.normal_sampler, fragment_texture_coord);

    return normalize(tangent * (normal_map_value.x - 0.5) + bitangent * (normal_map_value.y - 0.5) + normal * normal_map_value.z);
}

float compute_visibility_ortho(sampler2DShadow sampler, mat4 vp, vec3 light_direction) {

    // Compute sample location
    vec3 sample_location = vec3(vp * vec4(fragment_position, 1.0));
    if (sample_location.x < 0.0 || sample_location.x > 1.0 || sample_location.y < 0.0 || sample_location.y > 1.0) {
        return 1.0;
    }

    // Compute bias
    float bias = compute_bias_normalized(fragment_normal, light_direction, sampler, sample_location.z) + 0.001;

    // Compute visibility (with poisson sampling and hardware-accelerated PCF)
    float visibility = 0.0;
    ivec2 texture_size = textureSize(sampler, 0);
    for (int i = 0; i < 4; i++){
        vec2 texture_coord = sample_location.xy + POISSON_DISK[i] / texture_size;
        visibility += 0.25 * texture(sampler, vec3(texture_coord, sample_location.z - bias));
    }
    return visibility;
}

float compute_visibility_perspective(sampler2DShadow sampler, mat4 vp, vec3 light_direction, float near, float far) {

    // Compute sample locations
    vec4 sample_location = vp * vec4(fragment_position, 1.0);
    sample_location.xyz /= sample_location.w;
    sample_location.xyz = sample_location.xyz * 0.5 + 0.5;

    // Early-exit outside of shadow map
    if (sample_location.x < 0.0 || sample_location.x > 1.0 || sample_location.y < 0.0 || sample_location.y > 1.0) {
        return 0.0;
    }

    // Compute bias
    float bias = compute_bias(fragment_normal, light_direction, sampler, sample_location.z, near, far);

    // Compute visibility (with poisson sampling and hardware-accelerated PCF)
    float visibility = 0.0;
    ivec2 texture_size = textureSize(sampler, 0);
    for (int i = 0; i < 4; i++){
        vec2 texture_coord = sample_location.xy + POISSON_DISK[i] / texture_size;
        visibility += 0.25 * texture(sampler, vec3(texture_coord, sample_location.z - bias));
    }
    return visibility;
}

float compute_first_layer_perspective(sampler2DShadow sampler, mat4 vp, vec3 light_direction, float near, float far) {

    // Compute sample locations
    vec4 sample_location = vp * vec4(fragment_position, 1.0);
    sample_location.xyz /= sample_location.w;
    sample_location.xyz = sample_location.xyz * 0.5 + 0.5;

    // Early-exit outside of shadow map
    if (sample_location.x < 0.0
        || sample_location.x > 1.0
        || sample_location.y < 0.0
        || sample_location.y > 1.0
    ) {
        return 0.0;
    }
    float bias = compute_bias(fragment_normal, light_direction, sampler, sample_location.z, near, far);

    // Compute visibility
    float visibility = texture(sampler, vec3(sample_location.xy, sample_location.z - bias));
    return visibility;
}

float compute_diffuse(vec3 normal, vec3 light_direction) {

    // Compute (normalized Lambertian) diffuse strength
    float normalization_factor = 1.0 / PI;
    float diffuse_strength = normalization_factor * max(0.0, dot(normal, light_direction));

    return diffuse_strength;
}

float compute_specular(vec3 normal, vec3 light_direction) {

    // Compute half vector
    vec3 view_direction = normalize(camera.position - fragment_position);
    vec3 half_vector = normalize(light_direction + view_direction);

    // Compute (normalized Blinn-Phong) specular strength
    float n = 4.0 * material.shininess;
    float normalization_factor = ((n + 2.0) + (n + 4.0)) / (8.0 * PI * (pow(2, -n / 2.0) + n));
    float specular_strength = normalization_factor * pow(dot(normal, half_vector), n);

    return clamp(specular_strength, 0.0, 1.0);
}

vec3 compute_ambient_color() {

    // Compute material diffuse color
    vec3 material_color;
    if (material.diffuse.textured) {
        material_color = vec3(texture(material.diffuse.sampler, fragment_texture_coord));
    } else {
        material_color = material.diffuse.color;
    }

    return AMBIENT_STRENGTH * material_color;
}

vec3 compute_diffuse_color(vec3 normal, vec3 light_direction, vec3 light_color) {
    
    float diffuse_strength = compute_diffuse(normal, light_direction);

    // Compute material diffuse color
    vec3 material_color;
    if (material.diffuse.textured) {
        material_color = vec3(texture(material.diffuse.sampler, fragment_texture_coord));
    } else {
        material_color = material.diffuse.color;
    }

    // Compute diffuse color
    return diffuse_strength * light_color * material_color;
}

vec3 compute_specular_color(vec3 normal, vec3 light_direction, vec3 light_color) {

    float specular_strength = compute_specular(normal, light_direction);
    
    // Compute specular material color
    vec3 material_color;
    if (material.specular.textured) {
        material_color = vec3(texture(material.specular.sampler, fragment_texture_coord));
    } else {
        material_color = material.specular.color;
    }

    // Compute specular color
    return specular_strength * light_color * material_color;
}

vec3 compute_directional_light_color(vec3 normal, DirectionalLight light) {

    // Early-exit when light is behind fragment
    if (dot(normal, light.direction) < 0.0) {
        return vec3(0.0);
    }

    // Shadow
    float visibility = compute_visibility_ortho(light.shadow_sampler, light.vp, light.direction);

    // Compute individual colors
    vec3 diffuse_color = compute_diffuse_color(normal, light.direction, light.color);
    vec3 specular_color = compute_specular_color(normal, light.direction, light.color);

    // Compute combined color
    return light.intensity * visibility * (diffuse_color + specular_color);
}

vec3 compute_point_light_color(vec3 normal, PointLight light) {

    // Compute light properties
    vec3 light_direction = normalize(light.position - fragment_position);
    float light_distance = distance(light.position, fragment_position);
    float light_strength = light.intensity / (4.0 * PI * light_distance * light_distance);

    // Early-exit when light is behind fragment
    if (dot(normal, light_direction) < 0.0) {
        return vec3(0.0);
    }

    // Compute individual colors
    vec3 diffuse_color = compute_diffuse_color(normal, light_direction, light.color);
    vec3 specular_color = compute_specular_color(normal, light_direction, light.color);

    // Compute combined color
    return light_strength * (diffuse_color + specular_color);
}

float compute_spot_light_cone(vec3 normal, SpotLight light) {

    // Compute fragment-to-light vectors
    vec3 light_direction = normalize(light.position - fragment_position);
    float light_distance = distance(light.position, fragment_position);
    light_distance += 1.0; // Adjust for distance to unit sphere

    // Early-exit when light is behind fragment
    if (dot(normal, light_direction) < 0.0) {
        return 0.0;
    }

    // Exit when fragment is behind light
    if (dot(fragment_position - light.position, light.direction) < 0.0) {
        return 0.0;
    }

    // Normalize light strength to adhere to cone surface
    float light_strength = light.intensity / (2.0 * PI * light_distance * light_distance) / (1.0 - cos(light.angle));

    // Create cone effect
    vec3 vector = fragment_position - light.position;
    vec3 projection = light.direction * dot(vector, light.direction);
    float distance = distance(vector, projection);
    float radius = length(projection) * tan(light.angle);
    light_strength *= max(0.0, 1.0 - distance / radius);

    // Compensate for light loss due to linear fall-off
    light_strength *= 3.0;

    return light_strength;
}

vec3 compute_spot_light_color(vec3 normal, SpotLight light) {

    float light_strength = compute_spot_light_cone(normal, light);

    // Early-exit when no light strength
    if (light_strength <= 0.0) {
        return vec3(0.0);
    }
    
    // Compute fragment-to-light vectors
    vec3 light_direction = normalize(light.position - fragment_position);

    // Shadow
    float visibility = compute_visibility_perspective(light.shadow_sampler, light.vp, light_direction, light.near, light.far);

    // Compute individual colors
    vec3 diffuse_color = compute_diffuse_color(normal, light_direction, light.color);
    vec3 specular_color = compute_specular_color(normal, light_direction, light.color);

    // Compute combined color
    return light_strength * visibility * (diffuse_color + specular_color);
}

bool compute_xray(XrayLight light) {
    // Compute fragment-to-light vectors
    vec3 light_direction = normalize(light.position - fragment_position);
    float light_distance = distance(light.position, fragment_position);
    light_distance += 1.0; // Adjust for distance to unit sphere

    // Exit when fragment is behind light
    if (dot(fragment_position - light.position, light.direction) < 0.0) {
        return false;
    }

    // Exit if position is outside cone
    if (length(cross(light.direction, fragment_position - light.position)) > tan(light.angle) * dot(light.direction, fragment_position - light.position)) {
        return false;
    }
    
    // Shadow
    float visibility = compute_first_layer_perspective(light.shadow_sampler, light.vp, light_direction, light.near, light.far);

    if (visibility > 0.5 && xrayable) {
        discard;
    }
    return visibility <= 0.5;
}

vec3 compute_light_color(vec3 normal) {
    vec3 color;
    color += compute_directional_light_color(normal, directional_light);
    color += compute_point_light_color(normal, point_light);
    color += compute_spot_light_color(normal, spot_light);
    return color;
}

float compute_diffuse_strength(vec3 normal) {
    float intensity = 0.0;

    float directional_visibility = compute_visibility_ortho(directional_light.shadow_sampler, directional_light.vp, directional_light.direction);
    if (directional_visibility > 0.0) {
        intensity += compute_diffuse(normal, directional_light.direction) * directional_light.intensity * directional_visibility;
    }

    //intensity += compute_diffuse(normal, normalize(point_light.position - fragment_position)) * point_light.intensity;

    float spot_cone = compute_spot_light_cone(normal, spot_light);
    float spot_visibility = compute_visibility_perspective(spot_light.shadow_sampler, spot_light.vp, normalize(spot_light.position - fragment_position), spot_light.near, spot_light.far);
    if (spot_cone > 0.0 && spot_visibility > 0.0) {
        intensity += compute_diffuse(normal, normalize(spot_light.position - fragment_position)) * spot_light.intensity * spot_visibility * spot_cone;
    }

    return intensity;
}

float compute_specular_strength(vec3 normal) {
    float intensity = 0.0;

    float directional_visibility = compute_visibility_ortho(directional_light.shadow_sampler, directional_light.vp, directional_light.direction);
    if (directional_visibility > 0.0) {
        intensity += compute_specular(normal, directional_light.direction) * directional_light.intensity * directional_visibility;
    }

    //intensity += compute_specular(normal, normalize(point_light.position - fragment_position)) * point_light.intensity;

    float spot_cone = compute_spot_light_cone(normal, spot_light);
    float spot_visibility = compute_visibility_perspective(spot_light.shadow_sampler, spot_light.vp, normalize(spot_light.position - fragment_position), spot_light.near, spot_light.far);
    if (spot_cone > 0.0 && spot_visibility > 0.0) {
        intensity += compute_specular(normal, normalize(spot_light.position - fragment_position)) * spot_light.intensity * spot_visibility * spot_cone;
    }

    return intensity;
}

vec3 compute_toon(vec3 normal) {
    
    float diffuse = compute_diffuse_strength(normal);
    float specular = compute_specular_strength(normal);
    
    float intensity = clamp(diffuse + specular, 0.001, 0.999);
    float eye_distance = clamp(length(fragment_position - camera.position) - 0.914, 0.001, 0.999);

    vec4 toon_color = texture(toon_map, vec2(1-intensity,eye_distance));

    return vec3(toon_color);
}

void main() {

    // Compute xray visibility
    bool toon = toon_enabled;
    if (toon) {
        toon = compute_xray(xray_light);
    }

    // Compute normal
    vec3 normal = compute_normal();

    // Compute color
    if (toon) {
        out_color = compute_toon(normal);
    } else {
        out_color = compute_ambient_color() + compute_light_color(normal);
    }
    out_color = clamp(out_color, 0.0, 1.0);
}
