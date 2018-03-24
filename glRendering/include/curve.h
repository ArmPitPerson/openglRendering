/// OpenGL - by Carl Findahl - 2018

/* 
 * Contains implementation of various curves that
 * can be drawn using control points. Examples are
 * quadratic and cubic bezier curves.
 */

#ifndef CURVE_H
#define CURVE_H

#include "linalg.h"
#include "vertex.h"
#include "buffer.h"
#include "vertexArray.h"

#include <vector>
#include <memory>

class Curve
{
public:
    Curve();

    // Add a control point to the curve
    void addControlPoint(const vec3& point);

    // Subscript the idx'th control point
    const vec3& operator[](std::size_t idx) const
    {
        return mControlPoints[idx];
    }
    
    // Subscript the idx'th control point
    vec3& operator[](std::size_t idx)
    {
        return mControlPoints[idx];
    }

    // Set the presiscion at which to draw the curve ( 16 * val ) points
    void setPresiscion(const int val);

    // Call to prepare OpenGL Draw Data
    void prepareDrawData();

    // Get the number of points in the curve
    const int getPointCount() const;

    // Bind curve for drawing
    void bind() const;

    // Unbind curve from drawing
    void unbind() const;

private:
    // The presicion of the curve (number of points is 16 * mPresiscion)
    int mPresiscion = 8;

    // The control points of the curve
    std::vector<vec3> mControlPoints;

    // The points along the curve
    std::vector<Vertex> mPoints;

    // The vertex buffer object
    std::unique_ptr<VertexBuffer> mVbo;

    // The vertex array object
    VertexArray mVao;
};

#endif // CURVE_H
