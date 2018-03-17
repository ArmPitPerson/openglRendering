#include "renderer.h"
#include "curve.h"
#include "shapes.h"
#include "renderBatch.h"
#include "vertexArray.h"

void Renderer::draw(const Curve& curve) const
{
    curve.bind();
    gl::DrawArrays(gl::POINTS, 0, curve.getPointCount());
}

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

void Renderer::drawInstanced(const Shape2D& shape, const int instanceCount)
{
    shape.bind();
    gl::DrawElementsInstanced(gl::TRIANGLES, shape.getIndexCount(), gl::UNSIGNED_INT, nullptr, instanceCount);
}

void Renderer::drawInstanced(const RenderBatch& batch, const int instanceCount)
{
    batch.bind();
    gl::DrawElementsInstanced(gl::TRIANGLES, batch.getIndexCount(), gl::UNSIGNED_INT, nullptr, instanceCount);
}

void Renderer::drawInstanced(const VertexArray& vao, const unsigned indexCount, const int instanceCount)
{
    vao.bind();
    gl::DrawElementsInstanced(gl::TRIANGLES, indexCount, gl::UNSIGNED_INT, nullptr, instanceCount);
}
