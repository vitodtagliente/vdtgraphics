/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/renderer_3d.h>

namespace graphics
{
	class Renderer3DGL final : public Renderer3D
	{
	public:

		Renderer3DGL(API* const api);
		virtual ~Renderer3DGL() override;
	};
}