#version 450 core

// Vertex Attributes
layout (location=0) in vec4 aPosition;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 aTexCoord;

uniform mat4 mvpMatrix;

// Uniform block with all required matrices
layout(std140, binding=1) uniform Matrices {
    mat4 modelView;
    mat4 projection;
} mats;

// Out Parameters
out vec4 fs_color;
out vec2 fs_texCoord;

// Main Func
void main() {
    gl_Position = mvpMatrix * vec4(aPosition.xy, 0.f, 1.f);
    fs_color = vec4(aColor, 1);
    fs_texCoord = aTexCoord;
}
