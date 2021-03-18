#version 450 core

struct Camera {
    vec3 position;
};

struct PointLight {
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

struct Component {
    bool textured;
    sampler2D sampler;
    vec3 color;
};

struct Material {
    Component diffuse;
    Component specular;
    float shininess;
};

layout(location = 1) uniform Camera camera;
layout(location = 2) uniform PointLight light;
layout(location = 5) uniform Material material;

layout(location = 0) in vec3 fragment_position;
layout(location = 1) in vec3 fragment_normal;
layout(location = 2) in vec2 fragment_texture_coord;

layout(location = 0) out vec3 out_color;

float PI = 3.14159;

vec3 compute_diffuse_color(vec3 normal, vec3 light_direction, float light_strength) {

    // Compute normalized (Lambertian) diffuse strength
    float diffuse_strength = max(0.0, dot(normal, light_direction)) / PI;

    // Compute diffuse color
    if (material.diffuse.textured) {
        vec3 sampled_color = vec3(texture(material.diffuse.sampler, fragment_texture_coord));
        return light_strength * diffuse_strength * light.diffuse * sampled_color;
    } else {
        return light_strength * diffuse_strength * light.diffuse * material.diffuse.color;
    }
}

vec3 compute_specular_color(vec3 normal, vec3 light_direction, float light_strength) {

    // Early-exit when lit from behind
    if (dot(normal, light_direction) < 0.0) {
        return vec3(0.0);
    }

    // Compute (normalized Blinn-Phong) specular strength (http://www.farbrausch.de/~fg/stuff/phong.pdf)
    vec3 view_direction = normalize(camera.position - fragment_position);
    vec3 half_vector = normalize(light_direction + view_direction);
    float n = 4.0 * material.shininess;
    float normalization_factor = ((n + 2.0) + (n + 4.0)) / (8.0 * PI * (pow(2, -n / 2.0) + n));
    float specular_strength = normalization_factor * pow(max(0.0, dot(normal, half_vector)), n);

    // Compute specular color
    if (material.specular.textured) {
        vec3 sampled_color = vec3(texture(material.specular.sampler, fragment_texture_coord));
        return light_strength * specular_strength * light.specular * sampled_color;
    } else {
        return light_strength * specular_strength * light.specular * material.specular.color;
    }
}

void main() {

    // Compute shared variables
    vec3 normal = normalize(fragment_normal);

    float light_distance = distance(light.position, fragment_position);
    float light_strength = 1.0 / (4.0 * PI * light_distance * light_distance);
    vec3 light_direction = normalize(light.position - fragment_position);

    // Compute individual colors
    vec3 diffuse_color = compute_diffuse_color(normal, light_direction, light_strength);
    vec3 specular_color = compute_specular_color(normal, light_direction, light_strength);

    // Compute final color
    out_color = diffuse_color + specular_color;
    out_color = clamp(out_color, 0.0, 1.0);
}
