/// OpenGL - by Carl Findahl - 2018

/*
 * Contains implementation of various
 * interpolations and blend functions.
 * Linear, quadratic, cubic etc.
 */

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <cmath>

// Applies a linear curve to t
const float blendLinear(const float t);

// Applies a quadratic function to t
const float blendQuadratic(const float t);

// Applies a sqrt function to t
const float blendSquareRoot(const float t);

// Applies a derivative smoothstep
const float blendSmoothStepI(const float t);

// Applies a double-derivative smoothstep
const float blendSmoothStepII(const float t);

// Typedef for blend function
using BlendFunctionPtr = const float(*)(const float);

 // Lerp all numeric types that support multiplication / subtraction / addition
template <typename T>
constexpr T lerp(const T a, const T b, const float t, BlendFunctionPtr f = blendLinear)
{
    return a * (1 - f(t)) + b * f(t);
}

// Make a quadratic blend between points a, b and c
template <typename T>
constexpr T quadratic(const T a, const T b, const T c, const float t, BlendFunctionPtr f = blendLinear)
{
    return lerp<T>(lerp<T>(a, b, t, f), lerp<T>(b, c, t, f), t);
}

// Make a cubic blend between points a, b, c and d
template <typename T>
constexpr T cubic(const T a, const T b, const T c, const T d, const float t, BlendFunctionPtr f = blendLinear)
{
    return lerp<T>(quadratic<T>(a, b, c, t, f), quadratic<T>(b, c, d, t, f), t, f);
}


#endif // INTERPOLATION_H
