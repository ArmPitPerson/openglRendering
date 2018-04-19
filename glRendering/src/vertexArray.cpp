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

VertexArray::VertexArray(VertexArray&& other) : mName(other.mName), mNextAttributeBinding(other.mNextAttributeBinding),
                                                mBufferBinding(other.mBufferBinding)
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
    mBufferBinding = other.mBufferBinding;
    mNextAttributeBinding = other.mNextAttributeBinding;
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
    gl::VertexArrayAttribBinding(mName, mNextAttributeBinding, mBufferBinding);
    gl::VertexArrayAttribFormat(mName, mNextAttributeBinding, size, type, (normalize ? gl::TRUE_ : gl::FALSE_), offset);
    gl::EnableVertexArrayAttrib(mName, mNextAttributeBinding);
    ++mNextAttributeBinding;
}

void VertexArray::addIntegerAttribute(int size, unsigned type, unsigned offset)
{
    gl::VertexArrayAttribBinding(mName, mNextAttributeBinding, mBufferBinding);
    gl::VertexArrayAttribIFormat(mName, mNextAttributeBinding, size, type, offset); // Integral Version
    gl::EnableVertexArrayAttrib(mName, mNextAttributeBinding);
    ++mNextAttributeBinding;
}

void VertexArray::removeLastAttribute()
{
    if (mNextAttributeBinding != 0)
    {
        --mNextAttributeBinding;
        gl::DisableVertexArrayAttrib(mName, mNextAttributeBinding);
    }
    else
    {
        logWarn("Attempted to remove an attribute, when there are no attributes!");
    }
}

void VertexArray::setBuffer(const VertexBuffer& vbo)
{
    gl::VertexArrayVertexBuffer(mName, mBufferBinding, vbo.name(), 0, sizeof(Vertex));
}

void VertexArray::setIndexBuffer(const IndexBuffer& ibo)
{
    gl::VertexArrayElementBuffer(mName, ibo.name());
}

void VertexArray::setBufferBinding(unsigned binding)
{
    mBufferBinding = binding;
}
