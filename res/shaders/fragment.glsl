#version 450 core

layout(location = 1) uniform bool material_diffuse_textured;
layout(location = 2) uniform sampler2D material_diffuse_sampler;
layout(location = 3) uniform vec3 material_diffuse_color;

layout(location = 4) uniform vec3 light_color_diffuse;
layout(location = 5) uniform vec3 light_position;

layout(location = 0) in vec3 fragment_position;
layout(location = 1) in vec3 fragment_normal;
layout(location = 2) in vec2 fragment_texture_coord;

layout(location = 0) out vec3 out_color;

void main() {

    float light_distance = distance(light_position, fragment_position);
    float light_strength = 1.0 / 4.0 / 3.14 / light_distance / light_distance;
    vec3 light_direction = normalize(light_position - fragment_position) / light_distance;
    float diffuse_strength = max(dot(normalize(fragment_normal), light_direction), 0.0);

    if (material_diffuse_textured) {
        out_color = light_color_diffuse * light_strength * diffuse_strength * vec3(texture(material_diffuse_sampler, fragment_texture_coord));
    } else {
        out_color = light_color_diffuse * light_strength * diffuse_strength * material_diffuse_color, 1.0;
    }
}
