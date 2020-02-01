/// Copyright (c) Vito Domenico Tagliente

#pragma once 

#include "vector2.h"

namespace math
{
	template <typename T>
	struct circle_t
	{
		vector2_t<T> position;
		T radius;

		circle_t()
			: position()
			, radius()
		{}

		circle_t(const vector2_t<T>& position, const T radius)
			: position(position)
			, radius(radius)
		{

		}

		circle_t(const T x, const T y, const T radius)
			: position(x, y)
			, radius(radius)
		{

		}

		circle_t(const circle_t& circle)
			: position(circle.position)
			, radius(circle.radius)
		{

		}

		bool operator== (const circle_t& circle) const
		{
			return position == circle.position
				&& radius == circle.radius;
		}

		bool operator!= (const circle_t& circle) const
		{
			return !(*this == circle);
		}

		bool intersects(const circle_t& circle) const
		{
			return position.distance(circle.position) < (radius + circle.radius);
		}

		// return true if a point is contained inside the circle
		bool intersects(const vector2_t<T>& point) const
		{
			return position.distance(point) <= radius;
		}

	};

	// circle types

	typedef circle_t<float> circle;
}