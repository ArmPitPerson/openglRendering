/// OpenGL Rendering by Carl Findahl (C) 2018

#ifndef ENUMS_H
#define ENUMS_H

// Texture repeat modes
enum class ETextureRepeatMode
{
    Repeat,
    MirrorRepeat,
    ClampToEdge,
    ClampToBorder,
    MirroredClampToEdge
};

// Texture filtering modes
enum class ETextureFilterMode
{
    NearestNearest,
    NearestLinear,
    LinearNearest,
    Trilinear
};

// Image Read / Write Mode
enum class EImageMode
{
    ReadOnly = 0x88B8,  // Defines the shader read/write access specifier
    WriteOnly = 0x88B9,
    ReadWrite = 0x88BA
};

#endif // ENUMS_H
