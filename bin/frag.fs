#version 450 core
layout (location=0) out vec4 color;

uniform float uTime;

in vec4 fs_color;
in vec2 fs_texCoord;

void main() {
    color = fs_color + vec4(vec3(clamp(cos(uTime), -.5, .5)), 0);
}
