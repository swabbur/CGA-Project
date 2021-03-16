#version 450 core

layout(location = 1) uniform vec3 camera_position;

layout(location = 2) uniform vec3 light_color_diffuse;
layout(location = 3) uniform vec3 light_color_specular;
layout(location = 4) uniform vec3 light_position;

layout(location = 5) uniform bool material_diffuse_textured;
layout(location = 6) uniform sampler2D material_diffuse_sampler;
layout(location = 7) uniform vec3 material_diffuse_color;

layout(location = 8) uniform bool material_specular_textured;
layout(location = 9) uniform sampler2D material_specular_sampler;
layout(location = 10) uniform vec3 material_specular_color;

layout(location = 11) uniform float material_shininess;

layout(location = 0) in vec3 fragment_position;
layout(location = 1) in vec3 fragment_normal;
layout(location = 2) in vec2 fragment_texture_coord;

layout(location = 0) out vec3 out_color;

float PI = 3.14159;

vec3 compute_diffuse_color(vec3 normal, vec3 light_direction, float light_strength) {

    // Compute normalized (Lambertian) diffuse strength
    float diffuse_strength = max(0.0, dot(normal, light_direction)) / PI;

    // Compute diffuse color
    if (material_diffuse_textured) {
        vec3 sampled_color = vec3(texture(material_diffuse_sampler, fragment_texture_coord));
        return light_strength * diffuse_strength * light_color_diffuse * sampled_color;
    } else {
        return light_strength * diffuse_strength * light_color_diffuse * material_diffuse_color;
    }
}

vec3 compute_specular_color(vec3 normal, vec3 light_direction, float light_strength) {

    // Compute (normalized Blinn-Phong) specular strength
    vec3 view_direction = normalize(camera_position - fragment_position);
    vec3 half_vector = normalize(light_direction + view_direction);
    float specular_strength = pow(max(0.0, dot(normal, half_vector)), 4.0 * material_shininess) / 2.0 / PI;

    // Compute specular color
    if (material_specular_textured) {
        vec3 sampled_color = vec3(texture(material_specular_sampler, fragment_texture_coord));
        return light_strength * specular_strength * light_color_specular * sampled_color;
    } else {
        return light_strength * specular_strength * light_color_specular * material_specular_color;
    }
}

void main() {

    // Compute shared variables
    vec3 normal = normalize(fragment_normal);

    float light_distance = distance(light_position, fragment_position);
    float light_strength = 1.0 / 4.0 / PI / light_distance / light_distance;
    vec3 light_direction = (light_position - fragment_position) / light_distance;

    // Compute individual colors
    vec3 diffuse_color = compute_diffuse_color(normal, light_direction, light_strength);
    vec3 specular_color = compute_specular_color(normal, light_direction, light_strength);

    // Compute final color
    out_color = diffuse_color + specular_color;
    out_color = clamp(out_color, 0.0, 1.0);
}
