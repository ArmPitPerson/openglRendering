#include "renderer.h"
#include "shapes.h"
#include "renderBatch.h"
#include "vertexArray.h"

void Renderer::draw(const Shape2D& shape) const
{
    shape.bind();
    gl::DrawElements(gl::TRIANGLES, shape.getIndexCount(), gl::UNSIGNED_INT, nullptr);
}

void Renderer::draw(const RenderBatch& batch) const
{
    batch.bind();
    gl::DrawElements(gl::TRIANGLES, batch.getIndexCount(), gl::UNSIGNED_INT, nullptr);
}

void Renderer::draw(const VertexArray& vao, const unsigned indexCount) const
{
    vao.bind();
    gl::DrawElements(gl::TRIANGLES, indexCount, gl::UNSIGNED_INT, nullptr);
}
