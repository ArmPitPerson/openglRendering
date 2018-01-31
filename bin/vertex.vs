#version 450 core
layout (location=0) in vec4 aPosition;
layout (location=1) in vec4 aColor;
layout (location=2) in vec2 aTexCoord;

uniform vec2 uMousePos;
uniform vec2 uWindowSize;

out vec4 fs_color;
out vec2 fs_texCoord;

void main() {
    gl_Position = aPosition - vec4(0.5f, -0.5f, 0, 0) + vec4((uMousePos - .5f) / uWindowSize, 0, 0);
    fs_color = aColor;
    fs_texCoord = aTexCoord;
}
