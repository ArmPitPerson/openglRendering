///  GL Rendering by Carl Findahl (C) 2018

#ifndef TEXTURE_H
#define TEXTURE_H

#include "enums.h"
#include "linalg.h"

#include <string>


/*
 * For loading and binding Textures to the OpenGL context.
 * Supports mipmap loading. Specify levels with level parameter.
 * Currently only does PNG loading.
 *
 * File naming convention is:
 *	MIP 0: filename.png
 *	MIP 1: filename_1.png
 *	MIP N: filename_N.png
 *
 * For Array Textures:
 *  Layer 0: filename-0.png
 *  Layer N: filename-n.png
 */
class Texture
{
public:
	Texture(const std::string& filepath, unsigned mipLevels = 1, unsigned arrayLevels = 1);

    // Move Constructor
    Texture(Texture&& other);

    // Move Assignment
    Texture& operator=(Texture&& other);

    // Copy Constructor
    Texture(const Texture& other);

    // Copy Assignment
    Texture& operator=(const Texture& other);

	~Texture();

	// Bind texture to the provided bindingPoint [0-32]
	void bind(const int bindingPoint = 0) const;

	// Unbind from the last bound binding point
	void unbind() const;

    // Get width and height of the texture at the given mipmap level
    const vec2i getSize(const unsigned level = 0) const;

	// Set the repeat mode of the texture
	void setRepeatMode(ETextureRepeatMode mode);

	// Set the minification filter for the texture
	void setFilterMode(ETextureFilterMode mode);

	// Get the OpenGL name of the texture
	const unsigned name() const { return mName; }

private:
    // Init the texture and create the appropriate GL Object
    void init();

    // Copies texture data from the other texture
    void copyTextureData(const Texture& other);

	// Load texture from file
	void loadFromFile(const std::string& basePath);

    // Load array texture from file
    void loadArrayFromFile(const std::string& basePath);

private:
	// OpenGL name
	unsigned mName = 0;

	// MipMap Levels
	unsigned mLevels = 0;

    // Array Depth
    unsigned mArrayLevels = 0;

	// Bound to binding point
	mutable unsigned mBindingPoint = 0;	
};


#endif // TEXTURE_H
