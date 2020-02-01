/// Copyright (c) Vito Domenico Tagliente

#pragma once
#pragma warning(disable : 4201)

#include <cassert>
#include <cmath>
#include <cstring>
#include "algorithm.h"
#include "matrix3.h"
#include "vector4.h"
#include "vector3.h"

namespace math
{
	template <typename T>
	struct matrix4_t
	{
		static const matrix4_t zero;
		static const matrix4_t identity;

		// num of rows
		const std::size_t rows = 4;
		// num of columns
		const std::size_t columns = 4;
		// matrix size
		const std::size_t length = rows * columns;

		// matrix data
		union
		{
			struct
			{
				T m00, m01, m02, m03;
				T m10, m11, m12, m13;
				T m20, m21, m22, m23;
				T m30, m31, m32, m33;
			};

			T data[4 * 4];
		};

		matrix4_t()
			: data()
		{
			std::memset(data, static_cast<int>(0), length * sizeof(T));
		}

		matrix4_t(const T value)
			: data()
		{
			std::memset(data, value, length * sizeof(T));
		}

		matrix4_t(
			const T m00, const T m01, const T m02, const T m03,
			const T m10, const T m11, const T m12, const T m13,
			const T m20, const T m21, const T m22, const T m23,
			const T m30, const T m31, const T m32, const T m33
		) : 
			m00(m00), m01(m01), m02(m02), m03(m03),
			m10(m10), m11(m11), m12(m12), m13(m13),
			m20(m20), m21(m21), m22(m22), m23(m23),
			m30(m30), m31(m31), m32(m32), m33(m33)
		{

		}

		matrix4_t(const matrix4_t& matrix)
			: data()
		{
			std::memcpy(data, matrix.data, length * sizeof(T));
		}

		std::size_t size() const
		{
			return length;
		}

		// get (i,j) element
		T& operator() (const unsigned int i, const unsigned int j)
		{
			// row major implementation
			return data[i + j * columns];
		}

		T operator() (const unsigned int i, const unsigned int j) const
		{
			return data[i + j * columns];
		}

		// transpose matrix
		matrix4_t transpose() const
		{
			matrix4_t MT;
			for (unsigned int j = 0; j < rows; j++)
			{
				for (unsigned int i = 0; i < columns; i++)
				{
					MT(i, j) = (*this)(j, i);
				}
			}
			return MT;
		}

		// sub matrix
		matrix3_t<T> minor(const unsigned int i, const unsigned int j) const
		{
			assert(i < columns && j < rows);
			matrix3_t result;

			for (unsigned int j = 0, _j = 0; j < rows; ++j)
			{
				if (j == j) continue;
				for (unsigned int i = 0, _i = 0; i < columns; ++i)
				{
					if (i == i) continue;
					result(_i, _j) = (*this)(i, j);
					++_i;
				}
				++_j;
			}
			return result;
		}

		matrix4_t inverse(bool& is_invertible) const
		{
			is_invertible = false;
			T d = determinant(*this);
			if (d != static_cast<T>(0.0)) {
				is_invertible = true;
				return adjugate() / d;
			}
			return *this;
		}

		// adjugate matrix
		matrix4_t adjugate() const
		{
			matrix4_t result;
			for (unsigned int j = 0; j < rows; ++j)
			{
				for (unsigned int i = 0; i < columns; ++i)
				{
					matrix3_t<T> currentMinor = minor(i, j);
					result(j, i) = static_cast<T>(std::pow(-1, i + 1)) * currentMinor.determinant();
				}
			}
			return result;
		}

		// determinant 
		T determinant() const
		{
			/* Laplace law */
			int j = 0;
			T result{};
			for (unsigned int i = 0; i < columns; ++i)
			{
				const matrix3_t<T> minor = m.minor(i, j);
				result += std::pow(-1, i + j) * minor.determinant();
			}
			return result;
		}

		// orthograpic pojection
		static matrix4_t orthographic(
			const float left,
			const float right,
			const float bottom,
			const float top,
			const float near_plane,
			const float far_plane);

		// perspective projection
		static matrix4_t perspective(
			const float fov,
			const float aspect,
			const float near_plane,
			const float far_plane);

		// translation matrix
		static matrix4_t translate(const vector3_t<T>& vector);

		// rotation
		static matrix4_t rotate_x(const float theta);
		static matrix4_t rotate_y(const float theta);
		static matrix4_t rotate_z(const float theta);
		static matrix4_t rotate(const vector3_t<T> & vector, const float theta);

		// scale
		static matrix4_t scale(const vector3_t<T>& vector);

		/* Operators overloading */

		matrix4_t& operator= (const matrix4_t & matrix)
		{
			// check for self-assignment
			if (this == &matrix)
				return *this;

			std::memcpy(data, matrix.data, length * sizeof(T));
			return *this;
		}

		bool operator== (const matrix4_t & matrix) const
		{
			return m00 == matrix.m00 && m01 == matrix.m01 && m02 == matrix.m02 && m03 == matrix.m03
				&& m01 == matrix.m01 && m11 == matrix.m11 && m12 == matrix.m12 && m13 == matrix.m13
				&& m02 == matrix.m02 && m21 == matrix.m21 && m22 == matrix.m22 && m23 == matrix.m23
				&& m03 == matrix.m03 && m31 == matrix.m31 && m32 == matrix.m32 && m33 == matrix.m33;
		}

		bool operator!= (const matrix4_t & matrix) const
		{
			return m00 != matrix.m00 || m01 != matrix.m01 || m02 != matrix.m02 || m03 != matrix.m03
				|| m01 != matrix.m01 || m11 != matrix.m11 || m12 != matrix.m12 || m13 != matrix.m13
				|| m02 != matrix.m02 || m21 != matrix.m21 || m22 != matrix.m22 || m23 != matrix.m23
				|| m03 != matrix.m03 || m31 != matrix.m31 || m32 != matrix.m32 || m33 != matrix.m33;
		}

		matrix4_t& operator+= (const matrix4_t & matrix)
		{
			for (unsigned int i = 0; i < length; i++)
				data[i] += matrix.data[i];
			return *this;
		}

		matrix4_t& operator-= (const matrix4_t & matrix)
		{
			for (unsigned int i = 0; i < length; i++)
				data[i] -= matrix.data[i];
			return *this;
		}

		matrix4_t& operator*= (const T scalar)
		{
			for (unsigned int i = 0; i < length; i++)
				data[i] *= scalar;
			return *this;
		}

		matrix4_t& operator/= (const T scalar)
		{
			assert(scalar != static_cast<T>(0.0));
			T f = static_cast<T>(1.0) / scalar;
			return (*this) *= f;
		}

		matrix4_t operator- () const
		{
			matrix4_t result;
			for (unsigned int i = 0; i < length; i++)
				result.data[i] = -data[i];
			return result;
		}

		matrix4_t operator+ (const matrix4_t & matrix) const
		{
			matrix4_t result;
			for (unsigned int i = 0; i < length; i++)
				result.data[i] = data[i] + matrix.data[i];
			return result;
		}

		matrix4_t operator- (const matrix4_t & matrix) const
		{
			matrix4_t result;
			for (unsigned int i = 0; i < length; i++)
				result.data[i] = data[i] - matrix.data[i];
			return result;
		}

		matrix4_t operator* (const T scalar) const
		{
			matrix4_t result;
			for (unsigned int i = 0; i < length; i++)
				result.data[i] = data[i] * scalar;
			return result;
		}

		matrix4_t operator/ (const T scalar) const
		{
			assert(scalar != static_cast<T>(0.0));
			T f = static_cast<T>(1.0) / scalar;
			return (*this) * f;
		}

		matrix4_t operator* (const matrix4_t& matrix) const
		{
			matrix4_t result;
			for (unsigned int j = 0; j < rows; ++j)
			{
				for (unsigned int y = 0; y < columns; ++y)
				{
					for (unsigned int i = 0; i < rows; ++i)
					{
						result(y, j) += (*this)(i, j) * matrix(y, i);
					}
				}
			}
			return result;
		}

		vector4_t<T> operator* (const vector4_t<T>& vector) const
		{
			vector4_t<T> result;
			for (unsigned int j = 0; j < rows; ++j)
			{
				T value{};
				for (unsigned int i = 0; i < columns; ++i)
				{
					value += (*this)(i, j) * vector[i];
				}
				result[j] = value;
			}
			return result;
		}
	};

	template<typename T>
	inline matrix4_t<T> matrix4_t<T>::orthographic(const float left, const float right, const float bottom, const float top, const float near_plane, const float far_plane)
	{
		matrix4_t<T> m = matrix4_t<T>::identity;

		const T two = static_cast<T>(2.0);

		m.m00 = two / (right - left);
		m.m11 = two / (top - bottom);
		m.m22 = -two / (far_plane - near_plane);

		m.m30 = -(right + left) / (right - left);
		m.m31 = -(top + bottom) / (top - bottom);
		m.m32 = -(far_plane + near_plane) / (far_plane - near_plane);

		return m;
	}
	
	template<typename T>
	inline matrix4_t<T> matrix4_t<T>::perspective(const float fov, const float aspect, const float near_plane, const float far_plane)
	{
		matrix4_t<T> m = matrix4_t<T>::identity;

		const T two = static_cast<T>(2.0);

		float top = near_plane * std::tan(fov / two);
		float bottom = -top;
		float right = top * aspect;
		float left = -top * aspect;

		m.m00 = (two * near_plane) / (right - left);
		m.m11 = (two * near_plane) / (top - bottom);
		m.m22 = -(far_plane + near_plane) / (far_plane - near_plane);
		 
		m.m23 = -static_cast<T>(1.0);
		m.m32 = -(2.0f * near_plane * far_plane) / (far_plane - near_plane);

		return m;
	}

	template<typename T>
	inline matrix4_t<T> matrix4_t<T>::translate(const vector3_t<T>& vector)
	{
		matrix4_t<T> matrix = matrix4_t<T>::identity;

		matrix.m30 = vector.x;
		matrix.m31 = vector.y;
		matrix.m32 = vector.z;
		matrix.m33 = static_cast<T>(1.0);

		return matrix;
	}

	template<typename T>
	inline matrix4_t<T> matrix4_t<T>::rotate_x(const float theta)
	{
		const float rad = radians(theta);
		const float c = std::cos(rad);
		const float s = std::sin(rad);

		matrix4_t<T> matrix = matrix4_t<T>::identity;
		
		matrix.m11 = c;
		matrix.m21 = s;
		matrix.m12 = -s;
		matrix.m22 = c;

		return matrix;
	}

	template<typename T>
	inline matrix4_t<T> matrix4_t<T>::rotate_y(const float theta)
	{
		const float rad = radians(theta);
		const float c = std::cos(rad);
		const float s = std::sin(rad);

		matrix4_t<T> matrix = matrix4_t<T>::identity;

		matrix.m00 = c;
		matrix.m20 = -s;
		matrix.m02 = s;
		matrix.m22 = c;

		return matrix;
	}

	template<typename T>
	inline matrix4_t<T> matrix4_t<T>::rotate_z(const float theta)
	{
		const float rad = radians(theta);
		const float c = std::cos(rad);
		const float s = std::sin(rad);

		matrix4_t<T> matrix = matrix4_t<T>::identity;

		matrix.m00 = c;
		matrix.m10 = s;
		matrix.m01 = -s;
		matrix.m11 = c;

		return matrix;
	}

	template<typename T>
	inline matrix4_t<T> matrix4_t<T>::rotate(const vector3_t<T>& vector, const float theta)
	{
		const float rad = radians(theta);
		const float c = std::cos(rad);
		const float s = std::sin(rad);
		const float c1 = 1 - c;

		matrix4_t<T> matrix({
			std::pow(vector.x,2) * c1 + c,				vector.x * vector.y * c1 - vector.z * s,	vector.x * vector.normalize * c1 + vector.y * s,	0,
			vector.x * vector.y * c1 + vector.z * s,	std::pow(vector.y,2) * c1 + c,				vector.y * vector.z * c1 - vector.x * s,			0,
			vector.x * vector.y * c1 - vector.y * s,	vector.y * vector.z * c1 - vector.x * s,	std::pow(vector.z,2) * c1 + c,						0,
			0,												0,												0,														1
			});
		return matrix;
	}

	template<typename T>
	inline matrix4_t<T> matrix4_t<T>::scale(const vector3_t<T>& vector)
	{
		matrix4_t<T> matrix = matrix4_t<T>::identity;

		matrix.m00 = vector.x;
		matrix.m11 = vector.y;
		matrix.m22 = vector.z;

		return matrix;
	}

	template<typename T> const matrix4_t<T> matrix4_t<T>::zero = matrix4_t<T>(0.0);
	template<typename T> const matrix4_t<T> matrix4_t<T>::identity = matrix4_t<T>(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
		);

	// matrix types

	typedef matrix4_t<float> matrix4;
	typedef matrix4 mat4;

}