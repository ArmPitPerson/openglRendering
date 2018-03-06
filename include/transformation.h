/// OpenGL - by Carl Findahl - 2018

/* 
 * A transformation stores position,
 * rotation and scale data, and allows for
 * easy manipulation of these data.
 */

#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "linalg.h"

class Transformation final
{
public:
    Transformation() = default;

    void translate(const vec3& delta);

    void rotate(float angle, float x, float y, float z);

    void scale(const vec3& factor);

    // Get the transformation matrix
    const mat4& getTransform() const;

private:
    // Transform
    mat4 mTransform = mat4::identity();

};


#endif // TRANSFORMATION_H
