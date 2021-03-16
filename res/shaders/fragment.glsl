#version 450 core

layout(location = 1) uniform bool diffuse_textured;
layout(location = 2) uniform sampler2D diffuse_sampler;
layout(location = 3) uniform vec3 diffuse_color;

layout(location = 0) in vec2 texture_coord;

layout(location = 0) out vec4 out_color;

void main() {
    if (diffuse_textured) {
        out_color = texture(diffuse_sampler, texture_coord);
    } else {
        out_color = vec4(diffuse_color, 1.0);
    }
}
