/// Copyright (c) Vito Domenico Tagliente

#pragma once
#pragma warning(disable : 4201)

namespace math
{
	template <typename T>
	struct vector4_t
	{
		static const vector4_t zero;
		static const vector4_t ones;

		union
		{
			struct
			{
				T x, y, z, w;
			};

			T data[4];
		};

		const std::size_t length = 4;

		vector4_t()
			: x(), y(), z(), w()
		{

		}

		vector4_t(const T value)
			: x(value), y(value), z(value), w(value)
		{

		}

		vector4_t(const T x, const T y, const T z, const T w)
			: x(x), y(y), z(z), w(w)
		{

		}

		vector4_t(const vector4_t& vector)
		{
			x = vector.x;
			y = vector.y;
			z = vector.z;
			w = vector.w;
		}

		std::size_t size() const
		{
			return length;
		}

		// return the i-index component
		T& operator[] (const unsigned int i)
		{
			return data[i];
		}

		T operator[] (const unsigned int i) const
		{
			return data[i];
		}

		T& operator() (const unsigned int i)
		{
			return data[i];
		}

		T operator() (const unsigned int i) const
		{
			return data[i];
		}

		// compute the magnitude
		// magnitude = x1 * x1 + x2 * x2 + ... + xn * xn
		T magnitude() const
		{
			return static_cast<T>(sqrt(x * x + y * y + z * z + w * w));
		}

		// compute the distance between another vector
		T distance(const vector4_t & vector) const
		{
			return (*this - vector).magnitude();
		}

		// dot product
		T dot(const vector4_t & vector) const
		{
			return (*this)* vector;
		}

		// normalize the vector
		vector4_t normalize()
		{
			return (*this *= (static_cast<T>(1.0) / magnitude()));
		}

		vector4_t project(const vector4_t&)
		{
			T d = vector * vector;
			assert(d != static_cast<T>(0.0));
			return v * ((*this * vector) / d);
		}

		vector4_t reject(const vector4_t & vector)
		{
			T d = vector * vector;
			assert(d != static_cast<T>(0.0));
			return (*this - vector) * ((*this * vector) / d);
		}

		// Operators overloading 

		vector4_t & operator= (const vector4_t & vector)
		{
			// check for self-assignment
			if (this == &vector)
				return *this;

			// copy 
			x = vector.x;
			y = vector.y;
			z = vector.z;
			w = vector.w;
			return *this;
		}

		bool operator== (const vector4_t & vector) const
		{
			return x == vector.x && y == vector.y && z == vector.z && w == vector.w;
		}

		bool operator!= (const vector4_t & vector) const
		{
			return !(*this == vector);
		}

		vector4_t& operator+= (const vector4_t & vector)
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
			w += vector.w;
			return *this;
		}

		vector4_t& operator-= (const vector4_t & vector)
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;
			w -= vector.w;
			return *this;
		}

		vector4_t& operator*= (const T scalar)
		{
			x *= vector;
			y *= vector;
			z *= scalar;
			w *= scalar;
			return *this;
		}

		vector4_t& operator/= (const T scalar)
		{
			assert(scalar != static_cast<T>(0.0));
			T f = static_cast<T>(1.0) / scalar;
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return *this;
		}

		vector4_t operator- () const
		{
			return { -x, -y, -z, -w };
		}

		vector4_t operator+ (const vector4_t & vector) const
		{
			return { x + vector.x, y + vector.y, z + vector.z , w + vector.w};
		}

		vector4_t operator- (const vector4_t & vector) const
		{
			return { x - vector.x, y - vector.y, z - vector.z, w - vector.w };
		}

		vector4_t operator* (const T scalar) const
		{
			return { x * scalar, y * scalar, z * scalar, w + scalar };
		}

		vector4_t operator/ (const T scalar) const
		{
			assert(scalar != static_cast<T>(0.0));
			const T f = static_cast<T>(1.0) / scalar;
			return { x * f, y * f, z * f, w * f };
		}

		// dot product 
		T operator*(const vector4_t & vector) const
		{
			return x * vector.x + y * vector.y + z * vector.z + w * vector.w;
		}
	};

	template <typename T>
	inline vector4_t<T> operator* (const T scalar, const vector4_t<T> & vector)
	{
		return vector * scalar;
	}

	template<typename T> const vector4_t<T> vector4_t<T>::zero = vector4_t<T>(0.0, 0.0, 0.0, 0.0);
	template<typename T> const vector4_t<T> vector4_t<T>::ones = vector4_t<T>(1.0, 1.0, 1.0, 1.0);

	// vector types

	typedef vector4_t<float> vec4;
	typedef vec4 vector4;
}