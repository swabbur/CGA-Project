#version 450 core

uniform float aspect_ratio;
uniform float icon_scale;
uniform vec2 icon_position;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texture_coord;

layout(location = 0) out vec2 vertex_texture_coord;

void main() {

    // Pass values to fragment shader
    vertex_texture_coord = texture_coord;

    // Set output position
    gl_Position = vec4((icon_scale * position) + icon_position, 0.0, 1.0);
    gl_Position.x /= aspect_ratio;
}
