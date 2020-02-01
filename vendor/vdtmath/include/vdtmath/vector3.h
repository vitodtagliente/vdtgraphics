/// Copyright (c) Vito Domenico Tagliente

#pragma once
#pragma warning(disable : 4201)

namespace math
{
	template <typename T>
	struct vector3_t
	{
		static const vector3_t zero;
		static const vector3_t up;
		static const vector3_t right;
		static const vector3_t forward;
		static const vector3_t ones;

		union
		{
			struct
			{
				T x, y, z;
			};

			T data[3];
		};

		const std::size_t length = 3;

		vector3_t()
			: x(), y(), z()
		{

		}

		vector3_t(const T value)
			: x(value), y(value), z(value)
		{

		}

		vector3_t(const T x, const T y, const T z)
			: x(x), y(y), z(z)
		{

		}

		vector3_t(const vector3_t& vector)
		{
			x = vector.x;
			y = vector.y;
			z = vector.z;
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
			return static_cast<T>(sqrt(x * x + y * y + z * z));
		}

		// compute the distance between another vector
		T distance(const vector3_t & vector) const
		{
			return (*this - vector).magnitude();
		}

		// dot product
		T dot(const vector3_t & vector) const
		{
			return (*this) * vector;
		}

		//cross product
		vector3_t cross(const vector3_t & vector) const
		{
			return vector3_t(
				y * vector.z - z * vector.y,
				z * vector.x - x * vector.z,
				x * vector.y - y * vector.x
			);
		}

		// scalar triple product
		float triple(const vector3_t& vector1, const vector3_t& vector2) const
		{
			return ((*this).cross(vector1)) * vector2;
		}

		// normalize the vector
		vector3_t normalize()
		{
			return (*this *= (static_cast<T>(1.0) / magnitude()));
		}

		vector3_t project(const vector3_t &)
		{
			T d = vector * vector;
			assert(d != static_cast<T>(0.0));
			return v * ((*this * vector) / d);
		}

		vector3_t reject(const vector3_t & vector)
		{
			T d = vector * vector;
			assert(d != static_cast<T>(0.0));
			return (*this - vector) * ((*this * vector) / d);
		}

		// Operators overloading 

		vector3_t & operator= (const vector3_t & vector)
		{
			// check for self-assignment
			if (this == &vector)
				return *this;

			// copy 
			x = vector.x;
			y = vector.y;
			z = vector.z;
			return *this;
		}

		bool operator== (const vector3_t & vector) const
		{
			return x == vector.x && y == vector.y && z == vector.z;
		}

		bool operator!= (const vector3_t & vector) const
		{
			return !(*this == vector);
		}

		vector3_t& operator+= (const vector3_t & vector)
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
			return *this;
		}

		vector3_t& operator-= (const vector3_t & vector)
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;
			return *this;
		}

		vector3_t& operator*= (const T scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		vector3_t& operator/= (const T scalar)
		{
			assert(scalar != static_cast<T>(0.0));
			T f = static_cast<T>(1.0) / scalar;
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}

		vector3_t operator- () const
		{
			return { -x, -y, -z };
		}

		vector3_t operator+ (const vector3_t & vector) const
		{
			return { x + vector.x, y + vector.y, z + vector.z };
		}

		vector3_t operator- (const vector3_t & vector) const
		{
			return { x - vector.x, y - vector.y, z - vector.z };
		}

		vector3_t operator* (const T scalar) const
		{
			return { x * scalar, y * scalar, z * scalar };
		}

		vector3_t operator/ (const T scalar) const
		{
			assert(scalar != static_cast<T>(0.0));
			const T f = static_cast<T>(1.0) / scalar;
			return { x * f, y * f, z * f };
		}

		// dot product 
		T operator*(const vector3_t & vector) const
		{
			return x * vector.x + y * vector.y + z * vector.z;
		}
	};

	template <typename T>
	inline vector3_t<T> operator* (const T scalar, const vector3_t<T> & vector)
	{
		return vector * scalar;
	}

	template<typename T> const vector3_t<T> vector3_t<T>::zero = vector3_t<T>(0.0, 0.0, 0.0);
	template<typename T> const vector3_t<T> vector3_t<T>::up = vector3_t<T>(0.0, 1.0, 0.0);
	template<typename T> const vector3_t<T> vector3_t<T>::right = vector3_t<T>(1.0, 0.0, 0.0);
	template<typename T> const vector3_t<T> vector3_t<T>::forward = vector3_t<T>(0.0, 0.0, -1.0);
	template<typename T> const vector3_t<T> vector3_t<T>::ones = vector3_t<T>(1.0, 1.0, 1.0);

	// vector types

	typedef vector3_t<float> vec3;
	typedef vec3 vector3;
}