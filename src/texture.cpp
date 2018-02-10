#include "texture.h"
#include "gl_cpp.hpp"
#include "ext/stb_image.h"


Texture::Texture(const std::string& filepath) {
	gl::CreateTextures(gl::TEXTURE_2D, 1, &mName);
	gl::TextureParameteri(mName, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
	gl::TextureParameteri(mName, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	gl::TextureParameteri(mName, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
	gl::TextureParameteri(mName, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
	loadFromFile(filepath);
}

Texture::~Texture() {
	gl::DeleteTextures(1, &mName);
}

void Texture::bind(const int bindingPoint /*= 0*/) const {
	gl::ActiveTexture(gl::TEXTURE0 + bindingPoint);
	gl::BindTexture(gl::TEXTURE_2D, mName);
	mBindingPoint = bindingPoint;
}

void Texture::unbind() const {
	gl::ActiveTexture(gl::TEXTURE0 + mBindingPoint);
	gl::BindTexture(gl::TEXTURE_2D, 0);
}

void Texture::loadFromFile(const std::string& filepath) {
	int w, h, comp;
	unsigned char* imageData = stbi_load(filepath.c_str(), &w, &h, &comp, STBI_rgb_alpha);
	gl::TextureStorage2D(mName, 1, gl::RGBA8, w, h);
	gl::TextureSubImage2D(mName, 0, 0, 0, w, h, gl::RGBA, gl::UNSIGNED_BYTE, imageData);
	stbi_image_free(imageData);
}
