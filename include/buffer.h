///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef BUFFER_H
#define BUFFER_H

#include "gl_cpp.hpp"


/* Buffer Type Wrapper */
enum class EBufferType {
	Array,
//	AtomicCounter,
// 	CopyRead,
// 	CopyWrite,
// 	DispatchIndirect,
// 	DrawIndirect,
	Element,
// 	PixelPack,
// 	PixelUnpack,
// 	Query,
// 	ShaderStorage,
// 	Texture,
// 	TransformFeedback,
	Uniform
};


template<EBufferType type>
class Buffer {
public:
	Buffer() { gl::CreateBuffers(1, &mName); }

	~Buffer() { gl::DeleteBuffers(1, &mName); }

	Buffer(const void* data, ptrdiff_t dataSize);

	const unsigned name() const { return mName; }

	void bind() const;
	
	void unbind() const;

private:
	// The OpenGL Name
	unsigned mName = 0;

};

template<EBufferType type>
Buffer<type>::Buffer(const void* data, ptrdiff_t dataSize) {
	gl::CreateBuffers(1, &mName);
	gl::NamedBufferStorage(mName, dataSize, data, 0);
}

template<EBufferType type>
void Buffer<type>::bind() const {
	if constexpr(type == EBufferType::Array) {
		gl::BindBuffer(gl::ARRAY_BUFFER, mName);
	}
	else if constexpr (type == EBufferType::Element) {
		gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, mName);
	}
	else if constexpr(type == EBufferType::Uniform) {
		gl::BindBuffer(gl::UNIFORM_BUFFER, mName);
	}
	else {
		static_assert(false, "The binding is invalid!");
	}
}

template<EBufferType type>
void Buffer<type>::unbind() const {
	if constexpr(type == EBufferType::Array) {
		gl::BindBuffer(gl::ARRAY_BUFFER, 0);
	}
	else if constexpr (type == EBufferType::Element) {
		gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0);
	}
	else if constexpr(type == EBufferType::Uniform) {
		gl::BindBuffer(gl::UNIFORM_BUFFER, 0);
	}
	else {
		static_assert(false, "The binding is invalid!");
	}
}

/// Buffer Aliases

using VertexBuffer = Buffer<EBufferType::Array>;
using IndexBuffer = Buffer<EBufferType::Element>;
using UniformBuffer = Buffer<EBufferType::Uniform>;

///


#endif // BUFFER_H
