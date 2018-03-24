/// OpenGL - Carl Findahl - 2018

/*
 * A collective file of abstracted OpenGL buffers
 * like vertex buffers and uniform block buffers.
 * #TODO Consider extracting buffers in separate files!
 */

#ifndef BUFFER_H
#define BUFFER_H

#include "shader.h"

#include <string>
#include <memory>
#include <unordered_map>

#include "gl_cpp.hpp"
#include "spdlog/fmt/fmt.h"

class VertexBuffer
{
public:
    VertexBuffer()
    {
        gl::CreateBuffers(1, &mName);
    }

    // Construct from known data of size
    VertexBuffer(const void* data, ptrdiff_t dataSize)
    {
        gl::CreateBuffers(1, &mName);
        gl::NamedBufferStorage(mName, dataSize, data, 0);
    }

    // Copy Ctor
    VertexBuffer(const VertexBuffer& other)
    {
        resetBufferDataFromCopy(other);
    }

    // Copy Assignment
    VertexBuffer& operator=(const VertexBuffer& other)
    {
        if (this == &other) return *this;
        resetBufferDataFromCopy(other);

        return *this;
    }

    // Move Ctor
    VertexBuffer(VertexBuffer&& other) : mName(other.mName)
    {
        other.mName = 0;
    }

    // Move Assignment
    VertexBuffer& operator=(VertexBuffer&& other)
    {
        if (this == &other) return *this;
        
        // Destructor Work
        gl::DeleteBuffers(1, &mName);
        
        // Steal Resources
        mName = other.mName;
        other.mName = 0;        
        
        return *this;
    }

    ~VertexBuffer()
    {
        gl::DeleteBuffers(1, &mName);
    }

    // Bind the vertex buffer to the array buffer
    void bind() const
    {
        gl::BindBuffer(gl::ARRAY_BUFFER, mName);
    }

    // Unbind the vertex buffer from the array buffer
    void unbind() const
    {
        gl::BindBuffer(gl::ARRAY_BUFFER, 0);
    }

    // Return the OpenGL name of the buffer
    const unsigned name() const
    {
        return mName;
    }

private:
    // Clear current buffer data and re-initialize with data copied from source
    void resetBufferDataFromCopy(const VertexBuffer& source)
    {
        gl::DeleteBuffers(1, &mName);
        gl::CreateBuffers(1, &mName);

        int size;
        gl::GetNamedBufferParameteriv(source.mName, gl::BUFFER_SIZE, &size);
        gl::NamedBufferStorage(mName, size, nullptr, 0);
        gl::CopyNamedBufferSubData(source.mName, mName, 0, 0, size);
    }

private:
    // The OpenGL Name
    unsigned mName = 0;

};


class IndexBuffer
{
public:
    IndexBuffer()
    {
        gl::CreateBuffers(1, &mName);
    }
    
    IndexBuffer(const void* data, ptrdiff_t dataSize, unsigned count) : mCount(count)
    {
        gl::CreateBuffers(1, &mName);
        gl::NamedBufferStorage(mName, dataSize, data, 0);
    };

    IndexBuffer(IndexBuffer&& other) : mName(other.mName), mCount(other.mCount)
    {
        other.mName = 0;
    }

    IndexBuffer& operator=(IndexBuffer&& other)
    {
        if (this == &other) return *this;
        
        // Destructor Work
        gl::DeleteBuffers(1, &mName);
        
        // Steal Resources
        mCount = other.mCount;
        mName = other.mName;
        other.mName = 0;      
        other.mCount = 0;
        
        return *this;
    }

    ~IndexBuffer()
    {
        gl::DeleteBuffers(1, &mName);
    }

    // Return the OpenGL name of the buffer
    const unsigned name() const
    {
        return mName;
    }

    // Get the count of indices
    const unsigned getCount() const
    {
        return mCount;
    }

    // Bind to the element array buffer
    void bind() const
    {
        gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, mName);
    }

    // Unbind from the element array buffer
    void unbind() const
    {
        gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0);
    }

private:
    // The OpenGL Name
    unsigned mName = 0;

    // Number of indices
    unsigned mCount = 0;
};

class UniformBuffer
{
public:
    // Remember to set program if using this constructor
    UniformBuffer(ptrdiff_t size)
    {
        gl::CreateBuffers(1, &mName);
        gl::NamedBufferStorage(mName, size, nullptr, gl::DYNAMIC_STORAGE_BIT);
    };

    UniformBuffer(ptrdiff_t size, const Shader& program) : mProgram(program.name())
    {
        gl::CreateBuffers(1, &mName);
        gl::NamedBufferStorage(mName, size, nullptr, gl::DYNAMIC_STORAGE_BIT);
    };

    ~UniformBuffer()
    {
        gl::DeleteBuffers(1, &mName);
    }

    const unsigned name() const
    {
        return mName;
    }

    // Bind the given uniform block to the given bind point, default = 1
    void setUniformBlock(const std::string& blockName, const int bindPoint = 1)
    {
        mUniformBlock = blockName;
        unsigned blockIndex = gl::GetUniformBlockIndex(mProgram, mUniformBlock.data());
        gl::UniformBlockBinding(mProgram, blockIndex, bindPoint);
        mUniformOffsetCache.clear();
    }

    // Update the program
    void setProgram(const Shader& newProgram)
    {
        mProgram = newProgram.name();
    }

    // Set the data of the whole uniform block (see uniformBlocks.h for presets)
    void setBlockData(const void* data, ptrdiff_t dataSize)
    {
        gl::NamedBufferSubData(mName, 0, dataSize, data);
    }

    // Set the data for a subset of the uniform with the given name
    void setPartialBlockData(const std::string& uniformName, const void* data, ptrdiff_t dataSize)
    {
        if (mUniformOffsetCache.find(uniformName) == mUniformOffsetCache.end())
        {
            // Prepare the name of the uniform
            const std::string toGet = fmt::format("{}.{}", mUniformBlock, uniformName);
            const char* glStrToGet = toGet.data();

            // Get the index of the uniform
            unsigned uniformIndex;
            gl::GetUniformIndices(mProgram, 1, &glStrToGet, &uniformIndex);

            // Get the offset of that uniform
            int uniformOffset;
            gl::GetActiveUniformsiv(mProgram, 1, &uniformIndex, gl::UNIFORM_OFFSET, &uniformOffset);

            mUniformOffsetCache[uniformName] = uniformOffset;
        }

        // Make OpenGL Write the data to that location
        gl::NamedBufferSubData(mName, mUniformOffsetCache.at(uniformName), dataSize, data);
    }

    // Bind uniform buffer to given bind point, default = 1
    void bind(const int n = 1) const
    {
        gl::BindBufferBase(gl::UNIFORM_BUFFER, n, mName);
    }

    // Unbind uniform from given bind point, default = 1
    void unbind(const int n = 1) const
    {
        gl::BindBufferBase(gl::UNIFORM_BUFFER, n, 0);
    }

private:
    // The OpenGL Name
    unsigned mName = 0;

    // The bound program
    unsigned mProgram = 0;

    // The name of the uniform block this buffer will fill
    std::string mUniformBlock;

    // The cache of uniform block component indices
    std::unordered_map<std::string, int> mUniformOffsetCache;
};

class AtomicCounterBuffer
{
public:
    // Construct a buffer with size amount of atomic counters
    AtomicCounterBuffer(const unsigned size) : mSize(size)
    {
        reset();
    }

    ~AtomicCounterBuffer()
    {
        gl::DeleteBuffers(1, &mName);
    }

    // Reset the counter to 0
    void reset()
    {   // #TODO Do this more elegantly, as this feels awkward
        unbind();
        gl::DeleteBuffers(1, &mName);
        gl::CreateBuffers(1, &mName);
        std::unique_ptr<unsigned[]> data = std::make_unique<unsigned[]>(mSize);
        for (int i = 0; i != mSize; ++i) data[i] = 0u;
        gl::NamedBufferStorage(mName, sizeof(unsigned) * mSize, data.get(), 0);
    }

    // Bind to the given Atomic Counter Binding
    const void bind(const unsigned bindingPoint = 0) const
    {
        gl::BindBufferBase(gl::ATOMIC_COUNTER_BUFFER, bindingPoint, mName);
    }

    // Unbind from the given Atomic Counter Binding
    const void unbind(const unsigned bindingPoint = 0) const
    {
        gl::BindBufferBase(gl::ATOMIC_COUNTER_BUFFER, bindingPoint, 0);
    }

private:
    // The OpenGL Name
    unsigned mName = 0;

    // Number of Counters
    const unsigned mSize;
};

#endif // BUFFER_H
