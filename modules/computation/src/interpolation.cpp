#include "interpolation.h"

const float blendLinear(const float t)
{
    return t;
}

const float blendQuadratic(const float t)
{
    return t * t;
}

const float blendSquareRoot(const float t)
{
    return std::sqrt(t);
}

const float blendSmoothStepI(const float t)
{
    return t * t * (3 - (2 * t));
}

const float blendSmoothStepII(const float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

