/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <vdtmath/vector2.h>
#include <vdtmath/vector3.h>
#include <vdtmath/matrix4.h>

#include "color.h"
#include "texture_coords.h"
#include "texture_rect.h"

namespace graphics
{
	struct Vertex
	{
		math::vec3 position;
		Color color;

		static constexpr size_t size = 3 + 4;
	};

	struct TexturedVertex
	{
		math::vec3 position;
		Color color;
		TextureCoords uv;

		static constexpr size_t size = 3 + 4 + 2;
	};

	struct SpriteVertex
	{
		math::mat4 transform;
		Color color;
		TextureRect rect;

		static constexpr size_t size = 4 + 4 + 16;
	};

	enum class ShapeRenderStyle
	{
		fill,
		stroke
	};
}