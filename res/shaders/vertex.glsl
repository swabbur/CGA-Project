#version 450 core

layout(location = 0) uniform mat4 mvp;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec2 texture_coord;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    texture_coord = texCoord;
}
