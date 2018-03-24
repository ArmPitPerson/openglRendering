/// OpenGL - by Carl Findahl - 2018

/*
 * A batch of render data:
 * Vertices, Indices, Shaders, Uniform
 * Parameters for use with a renderer.
 * A batch can be submitted, and everything
 * in the render batch will only use a single
 * draw call.
 */

#ifndef BATCHRENDERER_H
#define BATCHRENDERER_H

#include "buffer.h"
#include "shapes.h"
#include "vertexArray.h"

#include <vector>
#include <memory>

class RenderBatch final
{
public:
    // Take control of a vao that already has attributes added to it
    RenderBatch(VertexArray&& vao);

    RenderBatch(RenderBatch&& other);

    RenderBatch& operator=(RenderBatch&& other);

    RenderBatch(const RenderBatch&) = delete;
    // #TODO Copy a render batch (Requires VAO, VBO and IBO to be copyable, if possible)

    RenderBatch& operator=(const RenderBatch&) = delete;

    // Clear the batched data and draw data
    void clear();

    // Push vertex and index data to the batch
    void push(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices);
    void push(const Shape2D& shape);

    // Commit the batch, finalizing it for rendering (must call before passing to a renderer)
    void commit();
    
    // Bind the batch to the context
    void bind() const;

    // Unbind the batch from the context
    void unbind() const;

    // Get number of indices in batch
    const unsigned getIndexCount() const;

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

    // Cleared since last draw
    bool bCommited = false;

    // The VAO used for drawing
    mutable VertexArray mVao;

    // The temporary VBO used between draw and clear calls (mutable since created before draw, but no logical difference)
    mutable std::unique_ptr<VertexBuffer> mVbo = nullptr;

    // The temporary IBO used between draw and clear calls  (mutable since created before draw, but no logical difference)
    mutable std::unique_ptr<IndexBuffer> mIbo = nullptr;
};

#endif // BATCHRENDERER_H
