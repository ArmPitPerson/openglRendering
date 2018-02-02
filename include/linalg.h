///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef LINALG_H
#define LINALG_H

#include <array>
#include <type_traits>


template<typename T, unsigned M>
class vecM {
public:
	using valueType = T;

	// Default Constructor
	constexpr vecM() = default;

	// All components have same value
	explicit vecM(T valueForAll) { mData.fill(valueForAll); }

	~vecM() = default;

	// Initialize Directly
	template<typename... U, typename = std::enable_if_t<(sizeof...(U) <= M)>>
	constexpr vecM(U... values) : mData({ static_cast<T>(values)... }) {}

	// Copy
	constexpr vecM(const vecM& other) : mData(other.mData) {}
	
	// Move
	constexpr vecM(vecM&& other) : mData(std::move(other.mData)) { }

	// Copy-Ass
	constexpr vecM& operator=(const vecM& other) { if (this == &other) return *this; mData = other.mData; return *this; }
	
	// Move-Ass
	constexpr vecM& operator=(vecM&& other) { if (this == &other) return *this; mData = std::move(other.mData); return *this; }

	// Add-Ass
	vecM& vecM::operator+=(const vecM& other) {
		for (unsigned i = 0; i != M; ++i)
			mData[i] += other[i];
		return *this;
	}

	// Add
	friend vecM operator+(vecM lhs, const vecM& rhs) { return lhs += rhs; }

	// Sub-Ass
	vecM& vecM::operator-=(const vecM& other) {
		for (unsigned i = 0; i != M; ++i)
			mData[i] -= other[i];
		return *this;
	}

	// Sub
	friend vecM operator-(vecM lhs, const vecM& rhs) { return lhs -= rhs; }

	// Scalar-Ass multiplication
	vecM& operator*=(T rhs) {
		for (unsigned i = 0; i != M; ++i)
			mData[i] *= rhs;
		return *this;
	}

	// Scalar multiplication
	friend vecM operator*(vecM lhs, T rhs) {
		return lhs *= rhs;
	}

	// Multiplication (dot product)
	friend constexpr const T operator*(const vecM& lhs, const vecM& rhs) {
		T sum{}; // Init to 0
		for (unsigned i = 0; i != M; ++i)
			sum += lhs[i] * rhs[i];
		return sum;
	}

	// Subscript
	constexpr T& operator[](std::size_t idx) { return mData[idx]; }
	constexpr const T& operator[](std::size_t idx) const { return mData[idx]; }

	// Get ptr to data
	const T* data() const { return mData.data(); }

private:
	// Vector Data
	std::array<T, M> mData;

};


/// Vector Type Aliases

using vec2 = vecM<float, 2>;
using vec3 = vecM<float, 3>;
using vec4 = vecM<float, 4>;
using vec2d = vecM<double, 2>;
using vec3d = vecM<double, 3>;
using vec4d = vecM<double, 4>;
using vec2i = vecM<int, 2>;
using vec3i = vecM<int, 3>;
using vec4i = vecM<int, 4>;
using vec2u = vecM<unsigned, 2>;
using vec3u = vecM<unsigned, 3>;
using vec4u = vecM<unsigned, 4>;

///

template<typename T, unsigned M>
class matM final {
public:
	using valueType = T;

	// Default
	constexpr matM() = default;

	~matM() {}
	
	// All components have the same value
	explicit matM(T valueForAll) { mData.fill(valueForAll); }
	
	// Initialize each component in format [row0col0 row0col1 row0col2 row1col0 ...]
	template<typename... U, typename = std::enable_if_t<(sizeof...(U) <= M * M)>>
	constexpr matM(U... values) : mData({ static_cast<T>(values)... }) { transpose(); }

	// Copy
	constexpr matM(const matM& other) : mData(other.mData) {}

	// Copy-Ass
	constexpr matM& operator=(const matM& other) { if (this == &other) return *this; mData = other.mData; return *this; }
	
	// Call operator (to get value at R, C)
	T& operator()(unsigned row, unsigned col) {
		return mData[row + col * M];
	}

	// Const call operator (to get value at R, C)
	const T& operator()(unsigned row, unsigned col) const {
		return mData[row + col * M];
	}

	// Move
	constexpr matM(matM&& other) : mData(std::move(other.mData)) {}
	
	// Move-Ass
	constexpr matM& operator=(matM&& other) { if (this == &other) return *this; mData = std::move(other.mData); return *this; }

	// Add-Ass
	matM& operator+=(const matM& rhs) {
		int(unsigned i = 0; i != mSize; ++i)
			mData[i] += rhs[i];
	}

	// Add
	friend matM operator+(matM lhs, const matM& rhs) {
		return lhs += rhs;
	}

	// Sub-Ass
	matM& operator-=(const matM& rhs) {
		int(unsigned i = 0; i != mSize; ++i)
			mData[i] += rhs[i];
	}

	// Sub
	friend matM operator-(matM lhs, const matM& rhs) {
		return lhs -= rhs;
	}

	// Multiply-Ass
	matM& operator*=(const matM& rhs) {
		std::array<T, M * M> newData;
		for (int i = 0; i != M; ++i) {			// Row
			for (int j = 0; j != M; ++j) {		// Column
				T sum{};			// Dot product
				for (int k = 0; k != M; ++k) {	// Component
					sum += mData(i, k) * rhs.mData(k, j);
				}
				newData(i, j) = sum;
			}
		}
		mData = std::move(newData);
		return *this;
	}

	// Multiply
	friend matM operator* (matM lhs, const matM& rhs) {
		return lhs *= rhs;
	}

	// Transpose matrix in place
	matM& transpose() {
		const auto mCopy = *this;			// Copy data
		for(int i = 0; i != M; ++i){			// Row
			for (int j = 0; j != M; ++j) {		// Column
				(*this)(i, j) = mCopy(j, i);	// Assign
			}
		}
		return *this;
	}

	// Return the zero matrix
	static matM zero() {
		return matM{ 0 };
	}

	// Return the identity matrix
	static matM identity() {
		matM outMatrix{ 0 };
		for (int i = 0; i != M; ++i)
			outMatrix(i, i) = 1;
		return outMatrix;
	}

	// Return the an orthographic projection matrix
	static matM orthographic(float left, float right, float bottom, float top, float close, float away) {
		static_assert(M == 4, "Can not create orthographic matrix for other sizes than 4x4 matrices");
		matM outMatrix{ 0 };
		outMatrix(0, 0) = 2.f / (right - left);
		outMatrix(1, 1) = 2.f / (top - bottom);
		outMatrix(2, 2) = 2.f / (close - away);
		outMatrix(3, 3) = 1.f;
		outMatrix(0, 3) = (left + right) / (left - right);
		outMatrix(1, 3) = (bottom + top) / (bottom - top);
		outMatrix(2, 3) = (close + away) / (away - close);
		return outMatrix;
	}

	// Get ptr to data
	const T* data() const { return mData.data(); }

private:
	// Matrix Data
	std::array<T, M * M> mData;

	// Size of the matrix data
	const unsigned mSize = M * M;

};

/// Matrix Type Aliases

using mat2 =  matM<float, 2>;
using mat3 =  matM<float, 3>;
using mat4 =  matM<float, 4>;
using mat2d = matM<double, 2>;
using mat3d = matM<double, 3>;
using mat4d = matM<double, 4>;
using mat2i = matM<int, 2>;
using mat3i = matM<int, 3>;
using mat4i = matM<int, 4>;
using mat2u = matM<unsigned, 2>;
using mat3u = matM<unsigned, 3>;
using mat4u = matM<unsigned, 4>;

///

#endif // LINALG_H
