#version 450 core
layout (location=0) out vec4 color;

uniform sampler2D tex;

in vec4 fs_color;
in vec2 fs_texCoord;

void main() {
    color = texture(tex, fs_texCoord);
}
