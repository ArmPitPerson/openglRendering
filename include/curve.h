/// OpenGL - by Carl Findahl - 2018

/* 
 * Contains implementation of various curves
 * and interpolations. Linear, quadratic,
 * cubic etc.
 */

#ifndef CURVE_H
#define CURVE_H

// Lerp all numeric types that support multiplication / subtraction / addition
template <typename T>
constexpr T lerp(const T a, const T b, const float t)
{
    return a * (1 - t) + b * t;
}

// Make a quadratic blend between points a, b and c
template <typename T>
constexpr T quadratic(const T a, const T b, const T c, const float t)
{
    return lerp<T>(lerp<T>(a, b, t), lerp<T>(b, c, t), t);
}

// Make a quadratic blend between points a, b and c
template <typename T>
constexpr T cubic(const T a, const T b, const T c, const T d, const float t)
{
    return quadratic<T>(lerp<T>(a, b, t), lerp<T>(b, c, t), lerp<T>(c, d, t), t);
}

#endif // CURVE_H
