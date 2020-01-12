/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "renderer.h"

namespace graphics
{
	class Renderer2D : public Renderer
	{
	public:

		Renderer2D(Context* const context);
		virtual ~Renderer2D() = default;
	};
}