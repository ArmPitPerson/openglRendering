/// OpenGL - by Carl Findahl - 2018

/* 
 * A renderer that can batch items that
 * use the same shader and uniforms.
 * Here is hoping this will work!
 */

#ifndef BATCHRENDERER_H
#define BATCHRENDERER_H

#include "buffer.h"
#include "shapes.h"
#include "vertexArray.h"

#include <vector>
#include <memory>

class BatchRenderer final
{
public:
    // Take control of a vao that already has attributes added to it
    BatchRenderer(VertexArray&& vao);

    /// No Copy / Move of Renderer
    BatchRenderer (const BatchRenderer&) = delete;
    BatchRenderer& operator=(const BatchRenderer&) = delete;
    BatchRenderer(BatchRenderer&&) = delete;
    BatchRenderer& operator=(BatchRenderer&&) = delete;
    /// No Copy / Move of Renderer

    // Clear the batched data and draw data
    void clear();

    // Push vertex and index data to the batch
    void push(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices);
    void push(const Shape2D& shape);

    // Draw the batched data
    void draw(const Shader& shader) const;

private:
    // Create a VBO/IBO from the provided draw data
    void makeDrawData() const;

private:
    // Vertices
    std::vector<Vertex> mVertices;

    // Indices
    std::vector<unsigned> mIndices;

    // Offset of next index
    unsigned mIndexOffset = 0;

    // The VAO used for drawing
    mutable VertexArray mVao;

    // Cleared since last draw
    mutable bool bHaveClearedSinceLastDraw = true;

    // The temporary VBO used between draw and clear calls (mutable since created before draw, but no logical difference)
    mutable std::unique_ptr<VertexBuffer> mVbo = nullptr;

    // The temporary IBO used between draw and clear calls  (mutable since created before draw, but no logical difference)
    mutable std::unique_ptr<IndexBuffer> mIbo = nullptr;

};

#endif // BATCHRENDERER_H
