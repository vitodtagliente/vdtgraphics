/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <memory>

#include "color.h"

namespace graphics
{
	class Texture;

	class RenderTarget
	{
	public:
		RenderTarget(int width, int height, const Color& color = Color::White);
		~RenderTarget();

		inline unsigned int id() const { return m_id; }
		inline bool isValid() const { return m_id != 0; }

		inline unsigned int getWidth() const { return m_width; }
		inline unsigned int getHeight() const { return m_height; }
		const Color& getColot() const { return m_color; }
		Texture* const getTexture() const { return m_texture.get(); }

	private:
		// framebuffer id
		unsigned int m_id;
		// texture id
		std::unique_ptr<Texture> m_texture;
		// stencil buffer
		unsigned int m_depthId;
		// framebuffer size
		int m_width, m_height;
		// clear color
		graphics::Color m_color;
	};
}