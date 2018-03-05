#include "batchRenderer.h"


BatchRenderer::BatchRenderer(VertexArray&& vao) : mVao(std::move(vao))
{
}

void BatchRenderer::clear()
{
    mVertices.clear();
    mIndices.clear();
    mIndexOffset = 0;
    bHaveClearedSinceLastDraw = true;
}

void BatchRenderer::push(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices)
{
    // Add indices
    for (auto& index : indices)
    {
        mIndices.push_back(index + mIndexOffset);
    }

    // Add vertices and increment the offset
    for (auto& vert : vertices)
    {
        mVertices.push_back(vert);
        ++mIndexOffset;
    }
}

void BatchRenderer::push(const Shape2D& shape)
{
    push(shape.mVertices, shape.mIndices);
}

void BatchRenderer::draw(const Shader& shader) const
{
    if (bHaveClearedSinceLastDraw)
    {
        makeDrawData();
        bHaveClearedSinceLastDraw = false;
    }
    shader.bind();
    mVao.bind();
    gl::DrawElements(gl::TRIANGLES, mIbo->getCount(), gl::UNSIGNED_INT, nullptr);
    // #TODO Set Uniforms??? Or use Materials
}

void BatchRenderer::makeDrawData() const
{
    mVbo = std::make_unique<VertexBuffer>(mVertices.data(), sizeof(Vertex) * mVertices.size());
    mIbo = std::make_unique<IndexBuffer>(mIndices.data(), sizeof(unsigned) * mIndices.size(), static_cast<unsigned>(mIndices.size()));
    
    mVao.setBuffer(*mVbo);
    mVao.setIndexBuffer(*mIbo);
}
