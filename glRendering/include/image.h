/// OpenGL Rendering by Carl Findahl (C) 2018

#ifndef IMAGE_H
#define IMAGE_H

#include "enums.h"
#include "linalg.h"

class Image
{
public:
    Image(const vec2i& size, EImageMode mode);
    
    Image(const Image& other);
    
    Image& operator=(const Image& other);

    Image(Image&& other);

    Image& operator=(Image&& other);

    ~Image();

    // Get the size of the image
    const vec2i getSize() const;

    // Bind the image to the given image binding point
    void bind(const unsigned bindingPoint = 0) const;

    // Unbind the image from the given image binding point
    void unbind(const unsigned bindingPoint = 0) const;

private:
    // Create and allocate storage for an image of size
    void init(const vec2i& size);

    // Copy size pixels from the texture source (works with textures and images alike)
    void copyPixelDataFrom(const Image& source, const vec2i& size);

private:
    // The read/write mode used by this image
    EImageMode mMode = EImageMode::ReadWrite;

    // OpenGL Name
    unsigned mName = 0;
};


#endif // IMAGE_H
