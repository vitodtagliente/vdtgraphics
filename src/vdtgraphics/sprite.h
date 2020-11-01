/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "texture.h"
#include <vdtmath/rectangle.h>
#include <vdtmath/transform.h>

using namespace math;

namespace graphics
{
	struct Sprite
	{
		Sprite();
		Sprite(Texture* const texture, const math::transform& transform);
		Sprite(Texture* const texture, const math::transform& transform, const math::rect& rect);
		Sprite(const Sprite& sprite);

		Sprite& operator=(const Sprite& sprite);

		Texture* texture;
		math::transform transform;
		math::rect rect;
	};
}