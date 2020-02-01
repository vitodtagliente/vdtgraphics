/// Copyright (c) Vito Domenico Tagliente

#pragma once 

#include "vector2.h"

namespace math
{
	template <typename T>
	struct rectangle_t
	{
		T x, y, width, height;

		rectangle_t()
			: x()
			, y()
			, width()
			, height()
		{}

		rectangle_t(const T x, const T y, const T width, const T height)
			: x(x)
			, y(y)
			, width(width)
			, height(height)
		{

		}

		bool operator== (const rectangle_t& other) const
		{
			return x == other.x && y == other.y
				&& width == other.width && height == other.height;
		}

		bool operator!= (const rectangle_t& other) const
		{
			return !(*this == other);
		}

		rectangle_t& operator*= (const T scalar)
		{
			x *= scalar;
			y *= scalar;
			width *= scalar;
			height *= scalar;
			return *this;
		}

		rectangle_t& operator/= (const T scalar)
		{
			const T factor = static_cast<T>(1) / scalar;
			x *= factor;
			y *= factor;
			width *= factor;
			height *= factor;
			return *this;
		}

		rectangle_t operator* (const T scalar)
		{
			return 
			{ 
				x * scalar, 
				y * scalar, 
				width * scalar, 
				height * scalar 
			};
		}

		rectangle_t operator/ (const T scalar)
		{
			const T factor = static_cast<T>(1) / scalar;
			return
			{
				x * factor,
				y * factor,
				width * factor,
				height * factor
			};
		}

		bool intersects(const rectangle_t& other) const
		{
			// #todo
			return false;
		}

	};

	// rectangle types

	typedef rectangle_t<float> rect;
	typedef rect rectangle;
}