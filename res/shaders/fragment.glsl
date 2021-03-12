#version 450 core

layout(location = 1) uniform sampler2D sampler;

layout(location = 0) in vec2 texture_coord;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = texture(sampler, 2 * texture_coord);
}
