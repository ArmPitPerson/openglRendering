#version 450 core
layout(origin_upper_left) in vec4 gl_FragCoord;

layout(location=0) out vec4 color;

in vec4 fs_color;
in vec2 fs_texCoord;

// Main Func
void main() {
    color = fs_color;
}
