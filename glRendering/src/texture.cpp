#include "texture.h"
#include "gl_cpp.hpp"
#include "logging.h"

#include <memory>
#include <experimental/filesystem>

#include "stb_image.h"

using namespace std::experimental;

// #TODO - Swap texture after construction

Texture::Texture(const std::string& filepath, unsigned mipLevels, unsigned arrayLevels) : mLevels(mipLevels), mArrayLevels(arrayLevels)
{
    init();

    // Only load if mipmap levels are valid
    if (mipLevels == 0) logErr("Texture mipmap levels can not be 0!");
    else if (mArrayLevels > 1) loadArrayFromFile(filepath);
    else loadFromFile(filepath);
}

Texture::Texture(Texture&& other) : mName(other.mName), mLevels(other.mLevels),
                                    mArrayLevels(other.mArrayLevels), mBindingPoint(other.mBindingPoint)
{
    // Make it not manage the GL Resource anymore
    other.mName = 0;
}

Texture::Texture(const Texture& other) : mLevels(other.mLevels), mArrayLevels(other.mArrayLevels)
{
    if (mArrayLevels > 1)
    {
        logWarn("Array textures can not currently be copied!");
    }
    else
    {
        init();
        copyTextureData(other);
    }
}

Texture& Texture::operator=(const Texture& other)
{
    // Don't need to do anything if copying to self
    if (this == &other) return *this;

    // Clean up currently managed texture
    gl::DeleteTextures(1, &mName);
    mName = 0;

    // Copy data from other
    mLevels = other.mLevels;
    mArrayLevels = other.mArrayLevels;

    // Copy texture data and make new GL name to manage
    if (mArrayLevels > 1)
    {
        logWarn("Array textures can not currently be copy assigned!");
    }
    else
    {
        init();
        copyTextureData(other);
    }

    return *this;
}

Texture& Texture::operator=(Texture&& other)
{
    // No point moving a temporary to itself
    if (this == &other) return *this;

    // Clean up any current textures
    gl::DeleteTextures(1, &mName);

    // Member-Wise Copy
    mName = other.mName;
    mLevels = other.mLevels;
    mArrayLevels = other.mArrayLevels;
    mBindingPoint = other.mBindingPoint;

    // Remove the GL Resource from other
    other.mName = 0;

    return *this;
}

Texture::~Texture()
{
    gl::DeleteTextures(1, &mName);
}

void Texture::bind(const int bindingPoint /*= 0*/) const
{
    gl::BindTextureUnit(bindingPoint, mName);
    mBindingPoint = bindingPoint;
}

void Texture::unbind() const
{
    gl::BindTextureUnit(mBindingPoint, 0);
}

const glm::ivec2 Texture::getSize(const unsigned level /*= 0*/) const
{
    glm::ivec2 dimensions;
    gl::GetTextureLevelParameteriv(mName, level, gl::TEXTURE_WIDTH, &dimensions.x);
    gl::GetTextureLevelParameteriv(mName, level, gl::TEXTURE_HEIGHT, &dimensions.y);
    return dimensions;
}

void Texture::setRepeatMode(ETextureRepeatMode mode)
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

void Texture::setFilterMode(ETextureFilterMode mode)
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

void Texture::init()
{
    if (mArrayLevels <= 1)
    {   // Create regular texture at 0 or 1 array level
        if (mArrayLevels == 0) logWarn("Array level should not be 0!");
        gl::CreateTextures(gl::TEXTURE_2D, 1, &mName);
    }
    else
    {   // Create Array texture when more than 1 array level
        gl::CreateTextures(gl::TEXTURE_2D_ARRAY, 1, &mName);
    }

    gl::TextureParameteri(mName, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR); // Trilinear
    gl::TextureParameteri(mName, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
    gl::TextureParameteri(mName, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
    gl::TextureParameteri(mName, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
}

void Texture::copyTextureData(const Texture& other)
{
    auto size = other.getSize();
    gl::TextureStorage2D(mName, mLevels, gl::RGBA8, size.x, size.y);

    // #TODO support Array Texture copying (with and without mipmaps)

    // For each mip-map layer, copy the contents
    for (int i = 0; i != mLevels; ++i)
    {
        // Get size info and allocate space for the texture
        size = other.getSize(i);
        const auto dataSize = size.x * size.y * 4;
        std::unique_ptr<unsigned char[]> imageData = std::make_unique<unsigned char[]>(dataSize);

        // Retrieve texture data and copy it to the new texture
        gl::GetTextureImage(other.mName, i, gl::RGBA, gl::UNSIGNED_BYTE, dataSize, imageData.get());
        gl::TextureSubImage2D(mName, i, 0, 0, size.x, size.y, gl::RGBA, gl::UNSIGNED_BYTE, imageData.get());
    }
}

void Texture::loadFromFile(const std::string& basePath)
{
    int width, height, comp;

    // Always load MipMap level 0
    unsigned char* mipBase = stbi_load(basePath.c_str(), &width, &height, &comp, STBI_rgb_alpha);
    gl::TextureStorage2D(mName, mLevels, gl::RGBA8, width, height);
    gl::TextureSubImage2D(mName, 0, 0, 0, width, height, gl::RGBA, gl::UNSIGNED_BYTE, mipBase);

    // Load any further MipMaps
    for (unsigned i = 1; i < mLevels; ++i)
    {
        // Find new path based on naming convention
        const std::string mipPath = basePath.substr(0, basePath.find(".png")) + "_" + std::to_string(i) + ".png";
        if (filesystem::v1::exists(mipPath)) // #TODO Swap to non-experimental when available
        {
            // Load and send data to OpenGL
            unsigned char* mipData = stbi_load(mipPath.c_str(), &width, &height, &comp, STBI_rgb_alpha);
            gl::TextureSubImage2D(mName, i, 0, 0, width, height, gl::RGBA, gl::UNSIGNED_BYTE, mipData);
            stbi_image_free(mipData);
        }
        else
        {
            // If path is invalid, then rely on auto-generated mip maps
            logWarn("Failed to load: {} auto-generating mip maps instead!", mipPath);
            gl::GenerateTextureMipmap(mName);
            break;
        }
    }

    stbi_image_free(mipBase);
}

void Texture::loadArrayFromFile(const std::string& basePath)
{
    // Load and fetch base image data
    int width, height, comp;
    unsigned char* imageData = stbi_load(basePath.c_str(), &width, &height, &comp, STBI_rgb_alpha);

    // Acquire Texture Storage and Assign level 0
    gl::TextureStorage3D(mName, mLevels, gl::RGBA8, width, height, mArrayLevels);
    gl::TextureSubImage3D(mName, 0, 0, 0, 0, width, height, 1, gl::RGBA, gl::UNSIGNED_BYTE, imageData);

    stbi_image_free(imageData);

    // #TODO Add support for Array Mip Maps

    // For every level starting at 1, load and assign the texture to the correct layer
    for (unsigned i = 1; i < mArrayLevels; ++i)
    {
        const auto levelPath = basePath.substr(0, basePath.find(".png")) + "-" + std::to_string(i) + ".png";
        imageData = stbi_load(levelPath.c_str(), &width, &height, &comp, STBI_rgb_alpha);
        gl::TextureSubImage3D(mName, 0, 0, 0, i, width, height, 1, gl::RGBA, gl::UNSIGNED_BYTE, imageData);
        stbi_image_free(imageData);
    }
}
