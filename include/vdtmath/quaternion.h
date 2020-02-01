/// Copyright (c) Vito Domenico Tagliente

#pragma once
#pragma warning(disable : 4201)

#include <cassert>
#include <cmath>
#include "vector.h"
#include "matrix4.h"

namespace math
{
	struct quaternion
	{
		union
		{
			struct
			{
				float x, y, z, w;
			};

			float data[4];
		};

		quaternion();
		quaternion(const vector3& vector, const float scalar);
		quaternion(const float _x, const float _y, const float _z, const float _w = 1.0f);

		float dot(const quaternion& quaternion) const;

		quaternion cross(const quaternion& quaternion) const;

		matrix4 matrix() const;

		vector4 axisAngle() const;

		// operators overloading

		quaternion& operator= (const quaternion& quaternion);

		bool operator== (const quaternion& quaternion) const;
		bool operator!= (const quaternion& quaternion) const;

		quaternion operator- ();

		quaternion operator+ (const quaternion& quaternion) const;

		quaternion& operator+= (const quaternion& quaternion);
		quaternion& operator-= (const quaternion& quaternion);

		quaternion operator* (const float scalar) const;

		quaternion& operator*= (const float scalar);

		quaternion operator/ (const float scalar) const;

		quaternion& operator/= (const float scalar);

		float length() const;

		quaternion normalize() const;

		quaternion inverse() const;

		float operator* (const quaternion& quaternion) const;
		vector3 operator* (const vector3& vector) const;
		vector4 operator* (const vector4& vector) const;
	};

	inline quaternion operator* (const float scalar, const quaternion& quaternion)
	{
		return quaternion * scalar;
	}
}