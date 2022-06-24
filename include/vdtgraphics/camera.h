/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <vdtmath/transform.h>

namespace graphics
{
	class Camera
	{
	public:
		Camera() = default;

		math::transform transform;

		virtual math::matrix4 getViewMatrix() const;
		virtual math::matrix4 getProjectionMatrix(int screenWidth, int screenHeight) const;

		void update();
	};
}