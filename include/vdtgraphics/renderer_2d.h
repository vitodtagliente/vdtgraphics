/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "renderer.h"
#include <vdtmath/vector2.h>
#include <vdtmath/matrix4.h>

namespace graphics
{
	class Renderer2D : public Renderer
	{
	public:

		Renderer2D(API* const api);
		virtual ~Renderer2D() = default;

		virtual void initialize() override;

		// draw a rectangle 
		void drawRect(const Color& color, const vector2& position);
		void drawRect(const Color& color, const vector2& position, const vector2& scale);
		void drawRect(const Color& color, const vector2& position, const float theta);
		void drawRect(const Color& color, const vector2& position, const float theta, const vector2& scale);
		void drawRect(const Color& color, const matrix4& transform);
		// draw circle
		void drawCircle(const Color& color, const vector2& position, const float radius);

	private:

		// generic 2d renderables
		Renderable* m_circle;
		Renderable* m_quad;
	};
}