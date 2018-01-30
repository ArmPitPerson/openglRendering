#version 450 core
layout (location=0) in vec4 aPosition;
layout (location=1) in vec4 aColor;
layout (location=2) in vec2 aTexCoord;

out vec4 fs_color;
out vec2 fs_texCoord;

void main() {
    gl_Position = aPosition;
    fs_color = aColor;
    fs_texCoord = aTexCoord;
}
