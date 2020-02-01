/// Copyright (c) Vito Domenico Tagliente

#pragma once
#pragma warning(disable : 4201)

#include <cassert>
#include <cmath>
#include <cstring>

namespace math
{
	template <typename T>
	struct matrix2_t
	{
		static const matrix2_t zero;
		static const matrix2_t identity;

		// num of rows
		const std::size_t rows = 2;
		// num of columns
		const std::size_t columns = 2;
		// matrix size
		const std::size_t length = 2 * 2;

		// matrix data
		union
		{
			struct
			{
				T m00, m01;
				T m10, m11;
			};

			T data[2 * 2];
		};

		matrix2_t()
			: data()
		{
			std::memset(data, static_cast<T>(0), length * sizeof(T));
		}

		matrix2_t(const T value)
			: data()
		{
			std::memset(data, value, length * sizeof(T));
		}

		matrix2_t(
			const T m00, const T m01,
			const T m10, const T m11
		) : 
			m00(m00), m01(m01),
			m10(m10), m11(m11)
		{

		}

		matrix2_t(const matrix2_t& matrix)
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
		matrix2_t transpose() const
		{
			matrix2_t MT;
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
		T minor(const unsigned int i, const unsigned int j) const
		{
			assert(i < columns && j < rows);
			return (*this)(i == 1 ? 0 : 1, j == 1 ? 0 : 1);
		}

		matrix2_t inverse(bool& is_invertible) const
		{
			is_invertible = false;
			T d = determinant(*this);
			if (d != static_cast<T>(0.0)) 
			{
				is_invertible = true;
				return matrix2_t(
					m11, -m10,
					-m01, m00
				) / d;
			}
			return *this;
		}

		// determinant 
		T determinant() const
		{
			return m00 * m11 - m01 * m10;
		}

		/* Operators overloading */

		matrix2_t& operator= (const matrix2_t & matrix)
		{
			// check for self-assignment
			if (this == &matrix)
				return *this;

			std::memcpy(data, matrix.data, length * sizeof(T));
			return *this;
		}

		bool operator== (const matrix2_t & matrix) const
		{
			return m00 == matrix.m00 && m01 == matrix.m01
				&& m01 == matrix.m01 && m11 == matrix.m11;
		}

		bool operator!= (const matrix2_t & matrix) const
		{
			return m00 != matrix.m00 || m01 != matrix.m01
				|| m01 != matrix.m01 || m11 != matrix.m11;
		}

		matrix2_t& operator+= (const matrix2_t & matrix)
		{
			for (unsigned int i = 0; i < length; i++)
				data[i] += matrix.data[i];
			return *this;
		}

		matrix2_t& operator-= (const matrix2_t & matrix)
		{
			for (unsigned int i = 0; i < length; i++)
				data[i] -= matrix.data[i];
			return *this;
		}

		matrix2_t& operator*= (const T scalar)
		{
			for (unsigned int i = 0; i < length; i++)
				data[i] *= scalar;
			return *this;
		}

		matrix2_t& operator/= (const T scalar)
		{
			assert(scalar != static_cast<T>(0.0));
			T f = static_cast<T>(1.0) / scalar;
			return (*this) *= f;
		}

		matrix2_t operator- () const
		{
			matrix2_t result;
			for (unsigned int i = 0; i < length; i++)
				result.data[i] = -data[i];
			return result;
		}

		matrix2_t operator+ (const matrix2_t & matrix) const
		{
			matrix2_t result;
			for (unsigned int i = 0; i < length; i++)
				result.data[i] = data[i] + matrix.data[i];
			return result;
		}

		matrix2_t operator- (const matrix2_t & matrix) const
		{
			matrix2_t result;
			for (unsigned int i = 0; i < length; i++)
				result.data[i] = data[i] - matrix.data[i];
			return result;
		}

		matrix2_t operator* (const T scalar) const
		{
			matrix2_t result;
			for (unsigned int i = 0; i < length; i++)
				result.data[i] = data[i] * scalar;
			return result;
		}

		matrix2_t operator/ (const T scalar) const
		{
			assert(scalar != static_cast<T>(0.0));
			T f = static_cast<T>(1.0) / scalar;
			return (*this) * f;
		}
	};

	template<typename T> const matrix2_t<T> matrix2_t<T>::zero = matrix2_t<T>(0.0);
	template<typename T> const matrix2_t<T> matrix2_t<T>::identity = matrix2_t<T>(
		1.0, 0.0,
		0.0, 1.0
		);

	// matrix types

	typedef matrix2_t<float> matrix2;
	typedef matrix2 mat2;
}