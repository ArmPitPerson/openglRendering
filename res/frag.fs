#version 450 core
layout(origin_upper_left) in vec4 gl_FragCoord;

layout (location=0) out vec4 color;
layout (binding=0, rgba8) uniform image2D image;
layout (binding=0) uniform sampler2D tex;

layout(std140) uniform Matrices {
    mat4 modelWorld;
    mat4 worldView;
    mat4 projection;
    vec4 drawColor;
    ivec4 cursorPosition;
} mats;

in vec4 fs_color;
in vec2 fs_texCoord;

void main() {
    if(mats.cursorPosition.z == 1){
        for(int i = 0; i < 9; ++i){
            for(int j = 0; j < 4; ++j) {
                imageStore(image, ivec2(mats.cursorPosition.xy) + ivec2(i + j, i), mats.drawColor);
                imageStore(image, ivec2(mats.cursorPosition.xy) + ivec2(-i + j, i), mats.drawColor);
            }
        }
    }

    // Current Color
    for(int x = 0; x < 25; ++x){
        for(int y = 0; y < 25; ++y){
            imageStore(image, ivec2(x, y), mats.drawColor);
        }
    }

    // Current Brush
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 4; ++j) {
            imageStore(image, ivec2(15, 35) + ivec2(i + j, i), mats.drawColor);
            imageStore(image, ivec2(15, 35) + ivec2(-i + j, i), mats.drawColor);
        }
    }

    memoryBarrier();
    color = imageLoad(image, ivec2(gl_FragCoord.xy)) + texture(tex, fs_texCoord);
}
