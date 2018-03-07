#include "renderBatch.h"
#include "logging.h"


RenderBatch::RenderBatch(VertexArray&& vao) : mVao(std::move(vao))
{
}

RenderBatch::RenderBatch(RenderBatch&& other) : mVertices(std::move(other.mVertices)),
                                                mIndices(std::move(other.mIndices)),
                                                mIndexOffset(other.mIndexOffset),
                                                bCommited(other.bCommited),
                                                mVao(std::move(other.mVao)),
                                                mVbo(std::move(other.mVbo)),
                                                mIbo(std::move(other.mIbo))
{
}

RenderBatch& RenderBatch::operator=(RenderBatch&& other)
{
    if (this == &other) return *this;

    // No Destructor Work

    // Steal Resources
    mVertices = std::move(other.mVertices);
    mIndices = std::move(other.mIndices);
    mIndexOffset = other.mIndexOffset;
    bCommited = other.bCommited;
    mVao = std::move(other.mVao);
    mVbo = std::move(other.mVbo);
    mIbo = std::move(other.mIbo);

    return *this;
}

void RenderBatch::clear()
{
    mVertices.clear();
    mIndices.clear();
    mIndexOffset = 0;
    bCommited = false;
}

void RenderBatch::push(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices)
{
    if (bCommited) logWarn("Pushing to committed render batch has no effect! "
                           "Please clear the batch before pushing more.");

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

void RenderBatch::push(const Shape2D& shape)
{
    push(shape.mVertices, shape.mIndices);
}

void RenderBatch::commit()
{
    if (bCommited) return;

    makeDrawData();
    bCommited = true;
}

void RenderBatch::bind() const
{
    mVao.bind();
}

void RenderBatch::unbind() const
{
    mVao.unbind();
}

const unsigned RenderBatch::getIndexCount() const
{
    return static_cast<unsigned>(mIndices.size());
}

void RenderBatch::makeDrawData() const
{
    mVbo = std::make_unique<VertexBuffer>(mVertices.data(), sizeof(Vertex) * mVertices.size());
    mIbo = std::make_unique<IndexBuffer>(mIndices.data(), sizeof(unsigned) * mIndices.size(), static_cast<unsigned>(mIndices.size()));

    mVao.setBuffer(*mVbo);
    mVao.setIndexBuffer(*mIbo);
}
