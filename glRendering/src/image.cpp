#include "image.h"

#include <memory>

#include "gl_cpp.hpp"

Image::Image(const glm::ivec2& size, EImageMode mode) : mMode(mode)
{
    init(size);
}

Image::Image(Image&& other) : mName(other.mName), mMode(other.mMode)
{
    other.mName = 0;
}

Image::Image(const Image& other) : mMode(other.mMode)
{
    auto size = other.getSize();
    init(size);
    copyPixelDataFrom(other, size);
}

Image& Image::operator=(const Image& other)
{
    if (this == &other) return *this;
    gl::DeleteTextures(1, &mName);

    mMode = other.mMode;

    // Copy data
    auto size = other.getSize();
    init(size);
    copyPixelDataFrom(other, size);

    return *this;
}

Image& Image::operator=(Image&& other)
{
    if (this == &other) return *this;
    gl::DeleteTextures(1, &mName);

    // Steal Data
    mMode = other.mMode;
    mName = other.mName;
    other.mName = 0;

    return *this;
}

Image::~Image()
{
    gl::DeleteTextures(1, &mName);
}

const glm::ivec2 Image::getSize() const {
    glm::ivec2 size;
    gl::GetTextureLevelParameteriv(mName, 0, gl::TEXTURE_WIDTH, &size.x);
    gl::GetTextureLevelParameteriv(mName, 0, gl::TEXTURE_HEIGHT, &size.y);
    return size;
}

void Image::bind(const unsigned bindingPoint /*= 0*/) const
{
    gl::BindImageTexture(bindingPoint, mName, 0, gl::FALSE_, 0, static_cast<GLenum>(mMode), gl::RGBA8);
}

void Image::unbind(const unsigned bindingPoint /*= 0*/) const
{
    gl::BindImageTexture(bindingPoint, 0, 0, gl::FALSE_, 0, static_cast<GLenum>(mMode), gl::RGBA8);
}

void Image::init(const glm::ivec2& size)
{
    // Create
    gl::CreateTextures(gl::TEXTURE_2D, 1, &mName);

    // Set Default Texture Parameters
    gl::TextureParameteri(mName, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR); // Trilinear
    gl::TextureParameteri(mName, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
    gl::TextureParameteri(mName, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
    gl::TextureParameteri(mName, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);

    // Allocate
    gl::TextureStorage2D(mName, 1, gl::RGBA8, size.x, size.y);
}

void Image::copyPixelDataFrom(const Image& source, const glm::ivec2& size)
{
    // Prepare storage
    const auto dataSize = size.x * size.y * 4; // Assumes RGBA8
    std::unique_ptr<unsigned char[]> pixels = std::make_unique<unsigned char[]>(dataSize);

    // Copy and paste image data
    gl::GetTextureImage(source.mName, 0, gl::RGBA, gl::UNSIGNED_BYTE, dataSize, pixels.get());
    gl::TextureSubImage2D(mName, 0, 0, 0, size.x, size.y, gl::RGBA8, gl::UNSIGNED_BYTE, pixels.get());
}
