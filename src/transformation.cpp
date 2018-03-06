#include "transformation.h"

void Transformation::translate(const vec3& delta)
{
    mTransform *= mat4::translate(delta[0], delta[1], delta[2]);
}

void Transformation::rotate(float angle, float x, float y, float z)
{
    mTransform *= mat4::rotate(angle, x, y, z);
}

void Transformation::scale(const vec3& factor)
{
    mTransform *= mat4::scale(factor[0], factor[1], factor[2]);
}

const mat4& Transformation::getTransform() const
{
    return mTransform;
}
