/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <vdtmath/transform.h>

namespace graphics
{
	struct Camera final
	{
		static math::mat4 view(const math::transform& transform);
		static math::mat4 view(math::transform& transform);
		static math::mat4 ortho(float nearPlane, float farPlane, int screenWidth, int screenHeight, float aspectRatio = 1.0f);
	};
}