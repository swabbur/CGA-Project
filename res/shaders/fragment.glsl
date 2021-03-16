#version 450 core

layout(location = 1) uniform bool material_diffuse_textured;
layout(location = 2) uniform sampler2D material_diffuse_sampler;
layout(location = 3) uniform vec3 material_diffuse_color;

layout(location = 9) uniform bool material_specular_textured;
layout(location = 10) uniform sampler2D material_specular_sampler;
layout(location = 11) uniform vec3 material_specular_color;

layout(location = 7) uniform float material_shininess;

layout(location = 4) uniform vec3 light_color_diffuse;
layout(location = 8) uniform vec3 light_color_specular;
layout(location = 5) uniform vec3 light_position;

layout(location = 6) uniform vec3 camera_position;

layout(location = 0) in vec3 fragment_position;
layout(location = 1) in vec3 fragment_normal;
layout(location = 2) in vec2 fragment_texture_coord;

layout(location = 0) out vec3 out_color;

float PI = 3.14159;

void main() {

    vec3 normal = normalize(fragment_normal);
    float light_distance = distance(light_position, fragment_position);
    vec3 light_direction = (light_position - fragment_position) / light_distance;
    vec3 view_direction = normalize(camera_position - fragment_position);
    vec3 half_vector = normalize(light_direction + view_direction);

    // Compute light strength based on sphere surface (e.g. how much of the light reaches this fragment)
    float light_strength = 1.0 / 4.0 / PI / light_distance / light_distance;

    // Compute normalized (Lambertian) diffuse factor
    float diffuse_strength = max(0.0, dot(normal, light_direction)) / PI;

    // Compute normalized (Blinn-Phong) specular factor
    float specular_strength = max(0.0, pow(dot(normal, half_vector), 4.0 * material_shininess) / 2.0 / PI);

    vec3 diffuse_color;
    if (material_diffuse_textured) {
        diffuse_color = light_color_diffuse * light_strength * diffuse_strength * vec3(texture(material_diffuse_sampler, fragment_texture_coord));
    } else {
        diffuse_color = light_color_diffuse * light_strength * diffuse_strength * material_diffuse_color, 1.0;
    }

    vec3 specular_color;
    if (material_specular_textured) {
        specular_color = light_color_specular * light_strength * specular_strength * vec3(texture(material_specular_sampler, fragment_texture_coord));
    } else {
        specular_color = light_color_specular * light_strength * specular_strength * material_specular_color, 1.0;
    }

    // Compute final color
    out_color = diffuse_color + specular_color;
    out_color = clamp(out_color, 0.0, 1.0);
}
