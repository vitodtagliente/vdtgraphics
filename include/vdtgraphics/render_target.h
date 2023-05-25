/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <memory>
#include <string>

#include "color.h"

namespace graphics
{
	class Texture;

	class RenderTarget
	{
	public:

		enum class State
		{
			Unknown,
			Error,
			Ready
		};

		RenderTarget(int width, int height, const Color& color = Color::White);
		~RenderTarget();

		void resize(int width, int height);

		inline unsigned int id() const { return m_id; }
		inline bool isValid() const { return m_id != 0; }

		inline unsigned int getWidth() const { return m_width; }
		inline unsigned int getHeight() const { return m_height; }

		const std::string& getErrorMessage() const { return m_errorMessage; }
		Texture* const getTexture() const { return m_texture.get(); }

		// clear color
		graphics::Color color;
	
	private:
		// framebuffer id
		unsigned int m_id;
		// texture id
		std::unique_ptr<Texture> m_texture;
		// stencil buffer
		unsigned int m_depthId;
		// framebuffer size
		int m_width, m_height;
		// the error message
		std::string m_errorMessage;
		// The state
		State m_state{ State::Unknown };
	};
}