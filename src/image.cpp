#include "image.h"

#include "gl_cpp.hpp"

Image::Image(const vec2& size, EImageMode mode) : mMode(mode)
{
    gl::CreateTextures(gl::TEXTURE_2D, 1, &mName);

    gl::TextureParameteri(mName, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR); // Trilinear
    gl::TextureParameteri(mName, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
    gl::TextureParameteri(mName, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
    gl::TextureParameteri(mName, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);

    gl::TextureStorage2D(mName, 1, gl::RGBA32F, size[0], size[1]);
}

Image::~Image()
{
    gl::DeleteTextures(1, &mName);
}

void Image::bind(unsigned bindingPoint /*= 0*/) const
{
    gl::BindImageTexture(bindingPoint, mName, 0, gl::FALSE_, 0, static_cast<GLenum>(mMode), gl::RGBA32F);
}

void Image::unbind(unsigned bindingPoint /*= 0*/) const
{
    gl::BindImageTexture(bindingPoint, 0, 0, gl::FALSE_, 0, static_cast<GLenum>(mMode), gl::RGBA32F);
}
