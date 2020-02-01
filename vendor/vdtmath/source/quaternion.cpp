#include <vdtmath/quaternion.h>

namespace math
{
	quaternion::quaternion()
		: x(), y(), z(), w(1.0f)
	{

	}

	quaternion::quaternion(const vector3& vector, const float scalar)
		: x(vector.x)
		, y(vector.y)
		, z(vector.z)
		, w(1.0f)
	{
	}

	quaternion::quaternion(const float _x, const float _y, const float _z, const float _w /* = 1.0f */)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w)
	{
	}

	float quaternion::dot(const quaternion& quaternion) const
	{
		return (*this) * quaternion;
	}

	quaternion quaternion::cross(const quaternion& quaternion) const
	{
		return {
			w * quaternion.x + x * quaternion.w + y * quaternion.z - z * quaternion.y,
			w * quaternion.y + y * quaternion.w + z * quaternion.x - x * quaternion.z,
			w * quaternion.z + z * quaternion.w + x * quaternion.y - y * quaternion.x,
			w * quaternion.w - x * quaternion.x + y * quaternion.y - z * quaternion.z
		};
	}

	matrix4 quaternion::matrix() const
	{
		const float xy = x * y;
		const float xz = x * z;
		const float yz = y * z;
		const float x2 = x * x;
		const float y2 = y * y;
		const float z2 = z * z;

		matrix4 result(
			1.0f - 2 * y2 - 2 * z2, 2 * xy + 2 * w * z, 2 * xz - 2 * w * y, 0.0f,
			2 * xy - 2 * w * z, 1 - 2 * x2 - 2 * z2, 2 * yz + 2 * w * z, 0.0f,
			2 * xz + 2 * w * y, 2 * yz - 2 * w * x, 1 - 2 * x2 - 2 * y2, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		return result;
	}

	vector4 quaternion::axisAngle() const
	{
		vector4 result;
		const float angle = 2.0f * std::acos(w);
		const float l = sqrt(1.0f - angle * angle);
		assert(l != 0.0f);
		const float f = 1.0f / l;
		result.x *= f;
		result.y *= f;
		result.z *= f;
		result.w = angle;
		return result;
	}

	// operators overloading

	quaternion& quaternion::operator= (const quaternion& quaternion)
	{
		// check for self-assignment
		if (this == &quaternion)
			return *this;

		// copy 
		x = quaternion.x;
		y = quaternion.y;
		z = quaternion.z;
		w = quaternion.w;
		return *this;
	}

	bool quaternion::operator== (const quaternion& quaternion) const
	{
		return x == quaternion.x && y == quaternion.y
			&& z == quaternion.z && w == quaternion.w;
	}

	bool quaternion::operator!= (const quaternion& quaternion) const
	{
		return x != quaternion.x || y != quaternion.y
			|| z != quaternion.z || w != quaternion.w;
	}

	quaternion quaternion::operator- ()
	{
		return { -x, -y, -z, -w };
	}

	quaternion quaternion::operator+ (const quaternion& quaternion) const
	{
		return {
			x + quaternion.x,
			y + quaternion.y,
			z + quaternion.z,
			w + quaternion.w
		};
	}

	quaternion& quaternion::operator+= (const quaternion& quaternion)
	{
		x += quaternion.x;
		y += quaternion.y;
		z += quaternion.z;
		w += quaternion.w;
		return *this;
	}

	quaternion& quaternion::operator-= (const quaternion& quaternion)
	{
		x -= quaternion.x;
		y -= quaternion.y;
		z -= quaternion.z;
		w -= quaternion.w;
		return *this;
	}

	quaternion quaternion::operator* (const float scalar) const
	{
		return { x * scalar, y * scalar, z * scalar, w * scalar };
	}

	quaternion& quaternion::operator*= (const float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	quaternion quaternion::operator/ (const float scalar) const
	{
		assert(scalar != 0.0f);
		const float f = 1.0f / scalar;
		return { x * f, y * f, z * f, w * f };
	}

	quaternion& quaternion::operator/= (const float scalar)
	{
		assert(scalar != 0.0f);
		const float f = 1.0f / scalar;
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}

	float quaternion::length() const
	{
		return std::sqrt(x * x + y * y + z * z + w * w);
	}

	quaternion quaternion::normalize() const
	{
		const float l = length();
		assert(l != 0.0f);
		return (*this) * (1.0f / l);
	}

	quaternion quaternion::inverse() const
	{
		const float l = length();
		assert(l != 0.0f);
		const float f = 1.0f / std::pow(l, 2);
		return quaternion(-x * f, -y * f, -z * f, w * f);
	}

	float quaternion::operator* (const quaternion& quaternion) const
	{
		return w * quaternion.w + x * quaternion.x + y * quaternion.y + z * quaternion.z;
	}

	vector3 quaternion::operator* (const vector3& vector) const
	{
		vector3 uv, uuv;
		vector3 qvec(x, y, z);
		uv = qvec.cross(vector);
		uuv = qvec.cross(uv);
		uv *= 2.0f * w;
		uuv *= 2.0f;

		return vector + uv + uuv;
	}

	vector4 quaternion::operator* (const vector4& vector) const
	{
		vector3 vec(vector.x, vector.y, vector.z);
		vector3 uv, uuv;
		vector3 qvec(x, y, z);
		uv = qvec.cross(vec);
		uuv = qvec.cross(uv);
		uv *= 2.0f * w;
		uuv *= 2.0f;

		const vector3 sum = vec + uv + uuv;

		return vector4(sum.x, sum.y, sum.z, 1.0f);
	}
}