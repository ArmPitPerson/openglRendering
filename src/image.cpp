#include "image.h"

#include "gl_cpp.hpp"

Image::Image(const vec2i& size, EImageMode mode) : mMode(mode)
{
    gl::CreateTextures(gl::TEXTURE_2D, 1, &mName);

    gl::TextureParameteri(mName, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR); // Trilinear
    gl::TextureParameteri(mName, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
    gl::TextureParameteri(mName, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
    gl::TextureParameteri(mName, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);

    gl::TextureStorage2D(mName, 1, gl::RGBA8, size[0], size[1]);    
}

Image::Image(Image&& other) : mName(other.mName), mMode(other.mMode)
{
    other.mName = 0;
}

Image::Image(const Image& other)
{
    // #TODO Implement using glGetTextureSubImage
}

Image& Image::operator=(Image&& other)
{
    if (this == &other) return *this;

    mMode = other.mMode;
    mName = other.mName;

    other.mName = 0;
    return *this;
}

Image::~Image()
{
    gl::DeleteTextures(1, &mName);
}

void Image::bind(unsigned bindingPoint /*= 0*/) const
{
    gl::BindImageTexture(bindingPoint, mName, 0, gl::FALSE_, 0, static_cast<GLenum>(mMode), gl::RGBA8);
}

void Image::unbind(unsigned bindingPoint /*= 0*/) const
{
    gl::BindImageTexture(bindingPoint, 0, 0, gl::FALSE_, 0, static_cast<GLenum>(mMode), gl::RGBA8);
}
