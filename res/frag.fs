#version 450 core
layout(origin_upper_left) in vec4 gl_FragCoord;

layout(location=0) out vec4 color;

// Standalone uniforms
layout(binding=0) uniform sampler2D tex;
layout(binding=1) uniform sampler2D hrise;

in vec4 fs_color;
in vec2 fs_texCoord;

void main() {
    color = mix(fs_color, texture(tex, fs_texCoord), 0.4f);
    color = mix(color, texture(hrise, fs_texCoord), 0.92f);
}
