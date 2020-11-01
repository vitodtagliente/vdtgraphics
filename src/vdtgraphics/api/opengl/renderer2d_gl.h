/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/renderer_2d.h>

namespace graphics
{
	class Renderer2DGL final : public Renderer2D
	{
	public:

		Renderer2DGL(API* const api);
		virtual ~Renderer2DGL() override;
	};
}