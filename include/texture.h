///  by Carl Findahl (C) 2018

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>


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


/*
 * For loading and binding Textures to the OpenGL context.
 * Supports mipmap loading. Specify levels with level parameter.
 * Currently only does PNG loading.
 *
 * File naming convention is:
 *	MIP 0: filename.png
 *	MIP 1: filename_1.png
 *	MIP N: filename_N.png
 */
class Texture
{
public:
	Texture(const std::string& filepath, unsigned levels = 1);
	~Texture();

	// Bind texture to the provided bindingPoint [0-32]
	void bind(const int bindingPoint = 0) const;

	// Unbind from the last bound binding point
	void unbind() const;

	// Set the repeat mode of the texture
	void setRepeatMode(ETextureRepeatMode mode);

	// Set the minification filter for the texture
	void setFilterMode(ETextureFilterMode mode);

	// Get the OpenGL name of the texture
	const unsigned name() const { return mName; }

private:
	// Load texture from file
	void loadFromFile(const std::string& filepath);

private:
	// OpenGL name
	unsigned mName = 0;

	// MipMap Levels
	unsigned mLevels = 0;

	// Bound to binding point
	mutable unsigned mBindingPoint = 0;	
};


#endif // TEXTURE_H
