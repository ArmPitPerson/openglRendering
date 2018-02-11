#include "texture.h"
#include "gl_cpp.hpp"
#include "ext/stb_image.h"
#include "logging.h"
#include <experimental/filesystem>

using namespace std::experimental;

Texture::Texture(const std::string& filepath, unsigned levels/*=1*/) : mLevels(levels)
{
	gl::CreateTextures(gl::TEXTURE_2D, 1, &mName);
	gl::TextureParameteri(mName, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR); // Trilinear
	gl::TextureParameteri(mName, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	gl::TextureParameteri(mName, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
	gl::TextureParameteri(mName, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);

	// Only load if it mipmap levels are valid
	if (levels == 0) logErr("Texture mipmap levels can not be 0!");
	else loadFromFile(filepath);
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

void Texture::loadFromFile(const std::string& filepath)
{
	int w, h, comp;
	
	// Always load MipMap level 0
	unsigned char* mipBase = stbi_load(filepath.c_str(), &w, &h, &comp, STBI_rgb_alpha);
	gl::TextureStorage2D(mName, mLevels, gl::RGBA8, w, h);
	gl::TextureSubImage2D(mName, 0, 0, 0, w, h, gl::RGBA, gl::UNSIGNED_BYTE, mipBase);
	
	// Load any further MipMaps
	for (unsigned i = 1; i < mLevels; ++i)
	{
		// Find new path based on naming convention
		const std::string mipPath = filepath.substr(0, filepath.find(".png")) + "_" + std::to_string(i) + ".png";
		if (filesystem::v1::exists(mipPath))
		{
			// Load and send data to OpenGL
			unsigned char* mipData = stbi_load(mipPath.c_str(), &w, &h, &comp, STBI_rgb_alpha);
			gl::TextureSubImage2D(mName, i, 0, 0, w, h, gl::RGBA, gl::UNSIGNED_BYTE, mipData);
			stbi_image_free(mipData);
		}
		else
		{
			// If path is invalid, then rely on auto-generated mip maps
			logCustom()->warn("Failed to load: {} auto-generating mip maps instead!", mipPath);
			gl::GenerateTextureMipmap(mName);
			break;
		}
	}
	
	stbi_image_free(mipBase);
}
