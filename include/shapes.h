/// OpenGL - by Carl Findahl - 2018

/* 
 * Shapes contains classes to easily create
 * various shape primitives in OpenGL.
 */

#ifndef SHAPES_H
#define SHAPES_H

#include "vertex.h"
#include "buffer.h"
#include "linalg.h"
#include "vertexArray.h"

#include <vector>
#include <memory>
#include <utility>

struct ShapeGLData
{
    ShapeGLData(VertexBuffer&& vb, IndexBuffer&& ib, VertexArray&& va) : vbo(std::move(vb)), ibo(std::move(ib)), vao(std::move(va)) {}

    VertexBuffer vbo;
    IndexBuffer ibo;
    VertexArray vao;
};

class Shape2D
{
public:
    virtual ~Shape2D() noexcept = default;

    // Batch renderer wants to access vertices / indices
    friend class RenderBatch;

    // Bind to render
    void bind() const;

    // Unbind
    void unbind() const;

    // Get the number of indices this shape requires
    const unsigned getIndexCount() const;

protected:
    // Add a vertex to the shape
    void addVertex(const vec2& pos, const vec3& col, const vec2& tc);

    // Add an index to be rendered
    void addIndex(const unsigned idx);

    // Add a series of provided indices to be rendered
    template<typename... Is>
    void addIndices(const Is... i);

    // Call after vertices/indices are added to commit data to OpenGL
    void init();

private:
    // Vertices
    std::vector<Vertex> mVertices;

    // Indices
    std::vector<unsigned> mIndices;

    // GL Data
    std::unique_ptr<ShapeGLData> mGLData = nullptr;
};

template<typename... Is>
void Shape2D::addIndices(const Is... i)
{
    (mIndices.push_back(i), ...);
}

class Quad : public Shape2D
{
public:
    Quad(const vec2& size, const vec3& col);

};

class Circle : public Shape2D
{
public:
    Circle(const float radius, const unsigned points, const vec3& col);
};

#endif // SHAPES_H
