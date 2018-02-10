///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef BUFFER_H
#define BUFFER_H

#include "shader.h"
#include <string>
#include "gl_cpp.hpp"
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
	UniformBuffer(ptrdiff_t size, const Shader& program) : mProgram(program.name()) {
		gl::CreateBuffers(1, &mName);
		gl::NamedBufferStorage(mName, size, nullptr , gl::DYNAMIC_STORAGE_BIT);
	};

	~UniformBuffer() { gl::DeleteBuffers(1, &mName); }

	const unsigned name() const { return mName; }

	// Bind the given uniform block to the given bind point, default = 1
	void setUniformBlock(const std::string& blockName, const int bindPoint = 1) {
		mUniformBlock = blockName;
 		unsigned blockIndex = gl::GetUniformBlockIndex(mProgram, mUniformBlock.data());
 		gl::UniformBlockBinding(mProgram, blockIndex, bindPoint);
	}

	// Update the program
	void setProgram(const Shader& newProgram) {
		mProgram = newProgram.name();
	}

	// Set the data of the whole uniform block (see uniformBlocks.h for presets)
	void setBlockData(const void* data, ptrdiff_t dataSize) {
		gl::NamedBufferSubData(mName, 0, dataSize, data);
	}

	// Set the data for a subset of the uniform with the given name
	void setPartialBlockData(const std::string& uniformName, const void* data, ptrdiff_t dataSize) {
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

	// Bind uniform buffer to given bind point, default = 1
	void bind(const int n = 1) const { gl::BindBufferBase(gl::UNIFORM_BUFFER, n, mName); }

	// Unbind uniform from given bind point, default = 1
	void unbind(const int n = 1) const { gl::BindBufferBase(gl::UNIFORM_BUFFER, n, 0); }

private:
	// The OpenGL Name
	unsigned mName = 0;

	// The bound program
	unsigned mProgram = 0;

	// The name of the uniform block this buffer will fill
	std::string mUniformBlock;
};


#endif // BUFFER_H
