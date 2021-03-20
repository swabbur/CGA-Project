#version 450 core

uniform mat4 mvp;
uniform mat4 position_transformation;
uniform mat3 normal_transformation;
uniform mat4 biased_shadow_mvp;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coord;

layout(location = 0) out vec3 vertex_position;
layout(location = 1) out vec3 vertex_normal;
layout(location = 2) out vec2 vertex_texture_coord;
layout(location = 3) out vec3 vertex_shadow_coord;

void main() {

    gl_Position = mvp * vec4(position, 1.0);

    vertex_position = vec3(position_transformation * vec4(position, 1.0));
    vertex_normal = normal_transformation * normal;
    vertex_texture_coord = texture_coord;
    vertex_shadow_coord = vec3(biased_shadow_mvp * vec4(vertex_position, 1.0));
}
