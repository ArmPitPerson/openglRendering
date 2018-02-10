#version 450 core
layout (location=0) in vec4 aPosition;
layout (location=1) in vec4 aColor;
layout (location=1) in vec2 aTexCoord;

layout(std140) uniform Matrices {
    mat4 modelView;
    mat4 projection;
} mats;

out vec4 fs_color;
out vec2 fs_texCoord;

void main() {
    gl_Position = mats.projection * mats.modelView * aPosition;
    fs_color = aColor;
    fs_texCoord = aTexCoord;
}
