#version 450 core
layout (location=0) in vec4 aPosition;
layout (location=1) in vec4 aColor;
layout (location=2) in vec2 aTexCoord;

layout(std140) uniform WorldInfo {
    float time;
    vec2 mousePos;
    vec2 windowSize;
    mat4 projectionMatrix;
    mat4 transformMatrix;
} world;

out vec4 fs_color;
out vec2 fs_texCoord;

void main() {
    gl_Position =  world.transformMatrix * aPosition;
    fs_color = aColor;
    fs_texCoord = aTexCoord;
}
