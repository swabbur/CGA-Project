#version 450 core

// Uniforms
uniform sampler2D icon_sampler;

// Inputs
layout(location = 0) in vec2 fragment_texture_coord;

// Outputs
layout(location = 0) out vec4 out_color;

void main() {

    // Sample icon texture
    out_color = texture(icon_sampler, fragment_texture_coord);
    out_color = clamp(out_color, 0.0, 1.0);
}
