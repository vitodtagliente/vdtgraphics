/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <string>

#include <vdtmath/vector2.h>

#include "color.h"
#include "font.h"

struct FONScontext;

namespace graphics
{
	class TextRenderer
	{
	public:
		TextRenderer();
		virtual ~TextRenderer();

		void init();

		void setColor(const Color& color);
		void setFont(const Font& font);
		void setSize(float size);

		void drawText(const std::string& text, const math::vec2& position);
		void drawText(const Font& font, const std::string& text, const math::vec2& position, const Color& color, float size);

	private:
		FONScontext* m_context{ nullptr };
	};
}