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
	struct GlyphVertex
	{
		math::vec3 positions[4];
		TextureCoords uvs[4];
		float offsetX = 0;
		float offsetY = 0;

		static constexpr size_t size = (3 * 4) + (2 * 4) + 1 + 1;
	};

	struct Vertex
	{
		math::vec3 position;
		Color color;

		static constexpr size_t size = 3 + 4;
	};

	struct SpriteVertex
	{
		math::mat4 transform;
		Color color;
		TextureRect rect;

		static constexpr size_t size = 4 + 4 + 16;
	};

	struct TexturedVertex
	{
		math::vec3 position;
		Color color;
		TextureCoords uv;

		static constexpr size_t size = 3 + 4 + 2;
	};


	enum class ShapeRenderStyle
	{
		fill,
		stroke
	};
}