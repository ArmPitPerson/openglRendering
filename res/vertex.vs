#version 450 core

// Vertex Attributes
layout (location=0) in vec4 aPosition;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 aTexCoord;

// Standalone uniforms
layout(binding=0) uniform sampler2D tex;

// Uniform block with all required matrices
layout(std140, binding=1) uniform Matrices {
    mat4 modelView;
    mat4 projection;
} mats;

// Out Parameters
out vec4 fs_color;
out vec2 fs_texCoord;

void main() {
    int firstBits = bitfieldExtract(gl_InstanceID, 0, 8);
    int lastBits = bitfieldExtract(gl_InstanceID, 8, 8);

    int randomSeed = gl_InstanceID * 66217493;
    int randomNumber = bitfieldExtract(randomSeed, 5, 8);

    vec4 colors = texture(tex, vec2(gl_InstanceID / 1.2f + randomNumber, gl_InstanceID - randomNumber));

    // Sample image
    vec4 adjustedPosition = aPosition;
    adjustedPosition.y *= colors.r * 1.5f;
    adjustedPosition.x += colors.g;
    adjustedPosition.z -= colors.b;

    gl_Position = mats.projection * mats.modelView * (adjustedPosition +
                                                      vec4(firstBits * 1.4f - (randomNumber + 1) + colors.r * 2.f,
                                                           0,
                                                           -lastBits + randomNumber - colors.r * 2.f,
                                                           0));
    fs_color = vec4(aColor, 1) + vec4(randomNumber / 256.f,
                                      -randomNumber / 256.f,
                                      0, 0);
    fs_texCoord = aTexCoord;
}
