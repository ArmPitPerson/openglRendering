#include "vertexArray.h"
#include "logging.h"
#include "vertex.h"

#include "gl_cpp.hpp"

VertexArray::VertexArray()
{
    gl::CreateVertexArrays(1, &mName);
}

VertexArray::VertexArray(const VertexBuffer& vbo)
{
    gl::CreateVertexArrays(1, &mName);
    setBuffer(vbo);
}

VertexArray::VertexArray(const VertexBuffer& vbo, const IndexBuffer& ibo)
{
    gl::CreateVertexArrays(1, &mName);
    setBuffer(vbo);
    setIndexBuffer(ibo);
}

VertexArray::VertexArray(VertexArray&& other) : mName(other.mName), mNextAttribute(other.mNextAttribute)
{
    other.mName = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other)
{
    if (this == &other) return *this;

    // Destructor Work
    gl::DeleteVertexArrays(1, &mName);

    // Steal Resources
    mName = other.mName;
    mNextAttribute = other.mNextAttribute;
    other.mName = 0;

    return *this;
}

VertexArray::~VertexArray()
{
    gl::DeleteVertexArrays(1, &mName);
}

void VertexArray::bind() const
{
    gl::BindVertexArray(mName);
}

void VertexArray::unbind() const
{
    gl::BindVertexArray(0);
}

const unsigned VertexArray::name() const
{
    return mName;
}

void VertexArray::addAttribute(int size, unsigned type, unsigned offset, bool normalize /*= false*/)
{
    gl::VertexArrayAttribBinding(mName, mNextAttribute, 0);
    gl::VertexArrayAttribFormat(mName, mNextAttribute, size, type, (normalize ? gl::TRUE_ : gl::FALSE_), offset);
    gl::EnableVertexArrayAttrib(mName, mNextAttribute);
    ++mNextAttribute;
}

void VertexArray::addIntegerAttribute(int size, unsigned type, unsigned offset)
{
    gl::VertexArrayAttribBinding(mName, mNextAttribute, 0);
    gl::VertexArrayAttribIFormat(mName, mNextAttribute, size, type, offset); // Integral Version
    gl::EnableVertexArrayAttrib(mName, mNextAttribute);
    ++mNextAttribute;
}

void VertexArray::removeLastAttribute()
{
    if (mNextAttribute != 0)
    {
        --mNextAttribute;
        gl::DisableVertexArrayAttrib(mName, mNextAttribute);
    }
    else
    {
        logWarn("Attempted to remove an attribute, when there are no attributes!");
    }
}

void VertexArray::setBuffer(const VertexBuffer& vbo)
{
    gl::VertexArrayVertexBuffer(mName, 0, vbo.name(), 0, sizeof(Vertex));
}

void VertexArray::setIndexBuffer(const IndexBuffer& ibo)
{
    gl::VertexArrayElementBuffer(mName, ibo.name());
}
