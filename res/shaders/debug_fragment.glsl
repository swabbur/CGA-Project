#version 450 core

layout(location = 0) out vec3 out_color;

void main() {
    out_color = vec3(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z);
}
