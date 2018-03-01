#version 450 core
layout (location=0) out vec4 color;

layout (binding=0) uniform sampler2DArray tex;

in vec4 fs_color;
in vec2 fs_texCoord;
in float fs_index;

void main() {
    color = texture(tex, vec3(fs_texCoord, fs_index));
}
