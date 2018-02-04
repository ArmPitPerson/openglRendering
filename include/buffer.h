///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef BUFFER_H
#define BUFFER_H

#include "gl_cpp.hpp"
#include <string>
#include "spdlog/fmt/fmt.h"



class VertexBuffer {
public:
	VertexBuffer() { gl::CreateBuffers(1, &mName); }

	~VertexBuffer() { gl::DeleteBuffers(1, &mName); }

	VertexBuffer(const void* data, ptrdiff_t dataSize) {
		gl::CreateBuffers(1, &mName);
		gl::NamedBufferStorage(mName, dataSize, data, 0);
	}

	void bind() const { gl::BindBuffer(gl::ARRAY_BUFFER, mName); }

	void unbind() const { gl::BindBuffer(gl::ARRAY_BUFFER, 0); }

	const unsigned name() const { return mName; }

private:
	// The OpenGL Name
	unsigned mName = 0;

};


class IndexBuffer {
public:
	IndexBuffer() { gl::CreateBuffers(1, &mName); }

	~IndexBuffer() { gl::DeleteBuffers(1, &mName); }

	IndexBuffer(const void* data, ptrdiff_t dataSize, unsigned count) : mCount(count) {
		gl::CreateBuffers(1, &mName);
		gl::NamedBufferStorage(mName, dataSize, data, 0);
	};

	const unsigned name() const { return mName; }

	const unsigned getCount() const { return mCount; }

	void bind() const { gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, mName); }

	void unbind() const { gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0); }

private:
	// The OpenGL Name
	unsigned mName = 0;

	// Number of indices
	unsigned mCount = 0;
};


class UniformBuffer {
public:
	UniformBuffer(ptrdiff_t size, unsigned program) : mProgram(program) {
		gl::CreateBuffers(1, &mName);
		gl::NamedBufferStorage(mName, size, nullptr , gl::DYNAMIC_STORAGE_BIT);
	};

	~UniformBuffer() { gl::DeleteBuffers(1, &mName); }

	const unsigned name() const { return mName; }

	void setUniformBlock(const std::string& blockName) {
		mUniformBlock = blockName;
		unsigned blockIndex = gl::GetUniformBlockIndex(mProgram, mUniformBlock.data());
		gl::UniformBlockBinding(mProgram, blockIndex, 1);
	}

	void setBlockData(const std::string& uniformName, const void* data, ptrdiff_t dataSize) {
		// Prepare the name of the uniform
		const std::string toGet = fmt::format("{}.{}", mUniformBlock, uniformName);
		const char* glStrToGet = toGet.data();
		
		// Get the index of the uniform
		unsigned uniformIndex;
		gl::GetUniformIndices(mProgram, 1, &glStrToGet, &uniformIndex);
		
		// Get the offset of that uniform
		int uniformOffset;
		gl::GetActiveUniformsiv(mProgram, 1, &uniformIndex, gl::UNIFORM_OFFSET, &uniformOffset);

		// Make OpenGL Write the data to that location
		gl::NamedBufferSubData(mName, uniformOffset, dataSize, data);
	}

	void bind() const { gl::BindBufferBase(gl::UNIFORM_BUFFER, 1, mName); }

	void unbind() const { gl::BindBufferBase(gl::UNIFORM_BUFFER, 1, 0); }

private:
	// The OpenGL Name
	unsigned mName = 0;

	// The bound program
	unsigned mProgram = 0;

	// The name of the uniform block this buffer will fill
	std::string mUniformBlock;
};


#endif // BUFFER_H
