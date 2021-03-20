#version 450 core

struct Camera {
    vec3 position;
};

struct Shadow {
    sampler2DShadow sampler;
    int size;
};

struct DirectionalLight {
    vec3 color;
    vec3 direction;
    float intensity;
    Shadow shadow;
    mat4 mvp;
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
    float intensity;
    float angle;
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
float PI = 3.14159;

vec2 poisson_disk[4] = vec2[](
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
uniform Material material;

// Inputs
layout(location = 0) in vec3 fragment_position;
layout(location = 1) in vec3 fragment_normal;
layout(location = 2) in vec2 fragment_texture_coord;

// Outputs
layout(location = 0) out vec3 out_color;

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

float compute_visibility(Shadow shadow, mat4 mvp, vec3 light_direction) {

    // Compute (slope-based) bias
    float light_angle = acos(max(0.0, dot(fragment_normal, light_direction)));
    float bias = clamp(0.001 * tan(light_angle), 0.0, 0.01);

    // Compute visibility (with poisson sampling and hardware-accelerated PCF)
    float visibility = 0.0;
    vec3 sample_location = vec3(mvp * vec4(fragment_position, 1.0));
    for (int i = 0; i < 4; i++){
        vec2 texture_coord = sample_location.xy + poisson_disk[i] / shadow.size;
        visibility += 0.25 * texture(shadow.sampler, vec3(texture_coord, sample_location.z - bias));
    }
    return visibility;
}

vec3 compute_diffuse_color(vec3 normal, vec3 light_direction, vec3 light_color) {

    // Compute (normalized Lambertian) diffuse strength
    float normalization_factor = 1.0 / PI;
    float diffuse_strength = normalization_factor * max(0.0, dot(normal, light_direction));

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

    // Compute half vector
    vec3 view_direction = normalize(camera.position - fragment_position);
    vec3 half_vector = normalize(light_direction + view_direction);

    // Compute (normalized Blinn-Phong) specular strength
    float n = 4.0 * material.shininess;
    float normalization_factor = ((n + 2.0) + (n + 4.0)) / (8.0 * PI * (pow(2, -n / 2.0) + n));
    float specular_strength = normalization_factor * pow(dot(normal, half_vector), n);

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
    float visibility = compute_visibility(light.shadow, light.mvp, light.direction);

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

vec3 compute_spot_light_color(vec3 normal, SpotLight light) {

    // Compute light properties
    vec3 light_direction = normalize(light.position - fragment_position);
    float light_distance = distance(light.position, fragment_position);
    float light_strength = light.intensity / (4.0 * PI * light_distance * light_distance);

    // Early-exit when light is behind fragment
    if (dot(normal, light_direction) < 0.0) {
        return vec3(0.0);
    }

    // Create cone effect
    float light_cosine = max(0.0, dot(-light_direction, light.direction));
    float inner_cosine = cos(light.angle / 2.0);

    if (light_cosine > inner_cosine) {
        light_strength *= 1.0;
    } else {
        light_strength *= 0.0;
    }

    // Compute individual colors
    vec3 diffuse_color = compute_diffuse_color(normal, light_direction, light.color);
    vec3 specular_color = compute_specular_color(normal, light_direction, light.color);

    // Compute combined color
    return light_strength * (diffuse_color + specular_color);
}

vec3 compute_light_color(vec3 normal) {
    vec3 color;
    color += compute_directional_light_color(normal, directional_light);
    color += compute_point_light_color(normal, point_light);
    color += compute_spot_light_color(normal, spot_light);
    return color;
}

void main() {

    // Compute normal
    vec3 normal = compute_normal();

    // Compute color
    out_color += compute_light_color(normal);
    out_color = clamp(out_color, 0.0, 1.0);
}
