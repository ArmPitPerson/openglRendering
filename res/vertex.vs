#version 450 core

// Vertex Attributes
layout (location=0) in vec4 aPosition;
layout (location=1) in vec4 aColor;
layout (location=2) in vec2 aTexCoord;

// Singular Uniforms
layout (binding=0, rgba8) uniform image2D image;
layout (binding=0) uniform atomic_uint counter;

// Uniform block with generic application info
layout(std140, binding=1) uniform AppData {
    vec2 cursorPosition;
    vec2 frameBufferSize;
    float timeSinceStart;
} appData;

// Uniform block with all required matrix operations
layout(std140, binding=2) uniform Matrices {
    mat4 modelWorld;
    mat4 worldView;
    mat4 projection;
} mats;

// Out Parameters
out vec4 fs_color;
out vec2 fs_texCoord;

void main() {
    gl_Position = aPosition;
    fs_color = aColor;
    fs_texCoord = aTexCoord;
}
