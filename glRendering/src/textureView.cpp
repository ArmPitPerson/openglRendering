#include "textureView.h"
#include "texture.h"

#include "gl_cpp.hpp"

TextureView::TextureView(const Texture& texture, unsigned mipmapCount)
{
    gl::GenTextures(1, &mName);
    setTexture(texture, mipmapCount);
    setFilterMode(ETextureFilterMode::Trilinear);
    setRepeatMode(ETextureRepeatMode::ClampToEdge);
}

TextureView::TextureView() {}

TextureView::TextureView(TextureView&& other) : mName(other.mName)
{
    other.mName = 0;
}

TextureView& TextureView::operator=(TextureView&& other)
{
    if (this == &other) return *this;

    // Clean up old texture and take ownership of the new one
    gl::DeleteTextures(1, &mName);
    mName = other.mName;
    other.mName = 0;

    return *this;
}

TextureView::~TextureView()
{
    gl::DeleteTextures(1, &mName);
}

void TextureView::bind(const unsigned bindingPoint /*= 0*/) const
{
    gl::BindTextureUnit(bindingPoint, mName);
}

void TextureView::unbind(const unsigned bindingPoint /*= 0*/) const
{
    gl::BindTextureUnit(bindingPoint, 0);
}

void TextureView::setTexture(const Texture& texture, unsigned mipmapCount)
{
    // Create a non-initialized texture
    if (mName == 0)
    {
        gl::GenTextures(1, &mName);
        gl::TextureView(mName, gl::TEXTURE_2D, texture.name(), gl::RGBA8, 0, mipmapCount, 0, 1);

        setFilterMode(ETextureFilterMode::Trilinear);
        setRepeatMode(ETextureRepeatMode::ClampToEdge);
    }
    else
    {
        // #TODO Read values and parameters from the texture to be more adaptable
        gl::TextureView(mName, gl::TEXTURE_2D, texture.name(), gl::RGBA8, 0, mipmapCount, 0, 1);
    }
}

void TextureView::setRepeatMode(ETextureRepeatMode mode)
{
    switch (mode)
    {
    case ETextureRepeatMode::Repeat:
        gl::TextureParameteri(mName, gl::TEXTURE_WRAP_S, gl::REPEAT);
        gl::TextureParameteri(mName, gl::TEXTURE_WRAP_T, gl::REPEAT);
        break;
    case ETextureRepeatMode::MirrorRepeat:
        gl::TextureParameteri(mName, gl::TEXTURE_WRAP_S, gl::MIRRORED_REPEAT);
        gl::TextureParameteri(mName, gl::TEXTURE_WRAP_T, gl::MIRRORED_REPEAT);
        break;
    case ETextureRepeatMode::ClampToEdge:
        gl::TextureParameteri(mName, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
        gl::TextureParameteri(mName, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
        break;
    case ETextureRepeatMode::ClampToBorder:
        gl::TextureParameteri(mName, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_BORDER);
        gl::TextureParameteri(mName, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_BORDER);
        break;
    case ETextureRepeatMode::MirroredClampToEdge:
        gl::TextureParameteri(mName, gl::TEXTURE_WRAP_S, gl::MIRROR_CLAMP_TO_EDGE);
        gl::TextureParameteri(mName, gl::TEXTURE_WRAP_T, gl::MIRROR_CLAMP_TO_EDGE);
        break;
    default:
        break;
    }
}

void TextureView::setFilterMode(ETextureFilterMode mode)
{
    switch (mode)
    {
    case ETextureFilterMode::NearestNearest:
        gl::TextureParameteri(mName, gl::TEXTURE_MIN_FILTER, gl::NEAREST_MIPMAP_NEAREST);
        break;
    case ETextureFilterMode::NearestLinear:
        gl::TextureParameteri(mName, gl::TEXTURE_MIN_FILTER, gl::NEAREST_MIPMAP_LINEAR);
        break;
    case ETextureFilterMode::LinearNearest:
        gl::TextureParameteri(mName, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_NEAREST);
        break;
    case ETextureFilterMode::Trilinear:
        gl::TextureParameteri(mName, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
        break;
    default:
        break;
    }
}
