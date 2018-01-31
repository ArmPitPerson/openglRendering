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

	constexpr vecM() = default;

	~vecM() = default;

	// Initialize Directly
	template<typename... U, typename V = std::enable_if_t<(sizeof...(U) <= M)>>
	constexpr vecM(U... values) : mData({ values... }) {}

	// Copy
	constexpr vecM(const vecM& other) : mData(other.mData) {}
	
	// Move
	constexpr vecM(vecM&& other) : mData(std::move(other.mData)) { }

	// Copy-Ass
	constexpr vecM& operator=(const vecM& other) { if (this == &other) return *this; mData = other.mData; return *this; }
	
	// Move-Ass
	constexpr vecM& operator=(vecM&& other) { if (this == &other) return *this; mData = std::move(other.mData); return *this; }

	// Add-Ass
	constexpr vecM& vecM::operator+=(const vecM& other) {
		for (unsigned i = 0; i != M; ++i)
			mData[i] += other[i];
		return *this;
	}

	// Add (TODO: Constexpr)
	friend vecM operator+(vecM lhs, const vecM& rhs) { return lhs += rhs; }

	// Sub-Ass
	constexpr vecM& vecM::operator-=(const vecM& other) {
		for (unsigned i = 0; i != M; ++i)
			mData[i] -= other[i];
		return *this;
	}

	// Sub (TODO: Constexpr)
	friend vecM operator-(vecM lhs, const vecM& rhs) { return lhs -= rhs; }

	// Multiplication (dot product)
	friend constexpr const T operator*(const vecM& lhs, const vecM& rhs) {
		T sum{}; // Init to 0
		for (unsigned i = 0; i != mData.size(); ++i)
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


/// Type Aliases

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
class matM {
public:
	constexpr matM() = default;

	template<typename... U, typename V = std::enable_if_t<(sizeof...(U) <= M * M)>>
	constexpr matM(U... values) : mData({ values... }) {}

	constexpr matM(const vecM<T, M>& col1, const vecM<T, M>& col2, const vecM<T, M>& col3, const vecM<T, M>& col4) {

	}

	// Get ptr to data
	const T* data() const { return mData.data(); }

private:
	// Matrix Data
	std::array<T, M * M> mData;

};


#endif // LINALG_H
