#version 450 core
layout (location=0) in vec4 aPosition;
layout (location=1) in vec4 aColor;
layout (location=2) in vec2 aTexCoord;

layout(std140) uniform Matrices {
    mat4 modelWorld[64];
    mat4 worldView;
    mat4 projection;
    int kaowaIndex;
} mats;

out vec4 fs_color;
out vec2 fs_texCoord;
out float fs_index;

void main() {
    gl_Position = mats.projection * mats.worldView * mats.modelWorld[mats.kaowaIndex] * aPosition;
    fs_color = aColor;
    fs_texCoord = aTexCoord;
    fs_index = float(mats.kaowaIndex % 12);
}
