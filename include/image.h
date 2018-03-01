/// OpenGL Rendering by Carl Findahl (C) 2018

#ifndef IMAGE_H
#define IMAGE_H

#include "enums.h"
#include "linalg.h"

class Image
{
public:
    Image(const vec2& size, EImageMode mode);

    ~Image();

    void bind(unsigned bindingPoint = 0) const;

    void unbind(unsigned bindingPoint = 0) const;

private:
    // The read/write mode used by this image
    EImageMode mMode;

    // OpenGL Name
    unsigned mName = 0;
};


#endif // IMAGE_H
