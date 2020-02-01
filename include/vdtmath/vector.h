/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

namespace math
{
	template <typename T>
	vector3_t<T> to_vector3(const vector2_t<T>& vector)
	{
		return { vector.x, vector.y, static_cast<T>(0.0) };
	}

	template <typename T>
	vector3_t<T> to_vec3(const vector2_t<T>& vector)
	{
		return { vector.x, vector.y, static_cast<T>(0.0) };
	}

	template <typename T>
	vector4_t<T> to_vector4(const vector2_t<T>& vector)
	{
		return { vector.x, vector.y, static_cast<T>(0.0), static_cast<T>(0.0) };
	}

	template <typename T>
	vector4_t<T> to_vector4(const vector3_t<T>& vector)
	{
		return { vector.x, vector.y, vector.z, static_cast<T>(0.0) };
	}

	template <typename T>
	vector4_t<T> to_vec4(const vector2_t<T>& vector)
	{
		return { vector.x, vector.y, static_cast<T>(0.0), static_cast<T>(0.0) };
	}

	template <typename T>
	vector4_t<T> to_vec4(const vector3_t<T>& vector)
	{
		return { vector.x, vector.y, vector.z, static_cast<T>(0.0) };
	}
}