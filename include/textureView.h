/// OpenGL - by Carl Findahl - 2018

/* 
 * Texture view is a way to represent a Texture
 * without loading any additional data to memory
 * It will read the data and act as if it was the
 * provided texture.
 */

#ifndef TEXTUREVIEW_H
#define TEXTUREVIEW_H

#include "enums.h"

class Texture;

class TextureView
{
public:
    TextureView();

    TextureView(const Texture& texture, unsigned mipmapCount = 1);

    // #TODO Copy / Move

    ~TextureView();

    // Bind texture view to the given texture binding point
    void bind(const unsigned bindingPoint = 0) const;

    // Unbind texture view from the given texture binding point
    void unbind(const unsigned bindingPoint = 0) const;

    // Change the texture this view will view
    void setTexture(const Texture& texture, unsigned mipmapCount = 1);

    // Set the repeat mode of the view
    void setRepeatMode(ETextureRepeatMode mode);
   
    // Set the filter mode of the view
    void setFilterMode(ETextureFilterMode mode);

private:
    // The OpenGL Name
    unsigned mName = 0;

};



#endif // TEXTUREVIEW_H
