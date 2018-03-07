#version 450 core

// Vertex Attributes
layout (location=0) in vec4 aPosition;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 aTexCoord;

// Uniform block with all required matrices
layout(std140, binding=1) uniform Matrices {
    mat4 modelView;
    mat4 projection;
} mats;

// Out Parameters
out vec4 fs_color;
out vec2 fs_texCoord;


void main() {
    gl_Position = mats.projection * mats.modelView * aPosition;
    fs_color = vec4(aColor, 1);
    fs_texCoord = aTexCoord;
}
