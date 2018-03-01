#version 450 core
layout (location=0) in vec4 aPosition;
layout (location=1) in vec4 aColor;
layout (location=2) in vec2 aTexCoord;

layout (binding=0, rgba8) uniform image2D image;

layout(std140) uniform Matrices {
    mat4 modelWorld;
    mat4 worldView;
    mat4 projection;
    vec4 drawColor;
    ivec4 cursorPosition;
} mats;

out vec4 fs_color;
out vec2 fs_texCoord;

void main() {
    gl_Position = mats.worldView * aPosition;
    fs_color = aColor;
    fs_texCoord = aTexCoord;
}
