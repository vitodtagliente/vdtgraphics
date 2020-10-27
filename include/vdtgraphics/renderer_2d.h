/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "renderer.h"
#include "sprite_batch.h"
#include <vdtmath/vector2.h>
#include <vdtmath/matrix4.h>

namespace graphics
{
	class Renderer2D : public Renderer
	{
	public:

		Renderer2D(API* const api);
		virtual ~Renderer2D() = default;

		// draw a rectangle 
		void drawRect(const Color& color, const vector2& position);
		void drawRect(const Color& color, const vector2& position, const vector2& scale);
		void drawRect(const Color& color, const vector2& position, const float theta);
		void drawRect(const Color& color, const vector2& position, const float theta, const vector2& scale);
		void drawRect(const Color& color, const matrix4& transform);
		// draw circle
		void drawCircle(const Color& color, const vector2& position, const float radius);

		void drawTexture(Texture* const texture, const vector2& position);

		virtual void render() override;

	private:

		void pushSpriteBatch(const SpriteBatch& spritebatch);

		// generic 2d renderables
		Renderable* m_circle;
		Renderable* m_quad;
		SpriteBatch m_spriteBatch;
	};
}