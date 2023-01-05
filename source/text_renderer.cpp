#include <vdtgraphics/text_renderer.h>

#define FONTSTASH_IMPLEMENTATION	// Expands implementation
#include <vdtgraphics/fontstash.h>

#include <glad/glad.h>
#define GLFONTSTASH_IMPLEMENTATION	// Expands implementation
#include <vdtgraphics/gl3corefontstash.h>

namespace graphics
{
	TextRenderer::TextRenderer()
	{
	}

	TextRenderer::~TextRenderer()
	{
		glfonsDelete(m_context);
	}

	void TextRenderer::init()
	{
		m_context = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);
	}

	void TextRenderer::setColor(const Color& color)
	{
		fonsSetColor(m_context, glfonsRGBA(color.red, color.green, color.blue, color.alpha));
	}

	void TextRenderer::setFont(const Font& font)
	{
		int id = fonsGetFontByName(m_context, font.path.string().c_str());
		if (id == FONS_INVALID)
		{
			id = fonsAddFontMem(m_context, font.path.string().c_str(), font.data.get(), font.size, 0);
		}

		if (id != FONS_INVALID)
		{
			fonsSetFont(m_context, id);
		}
	}

	void TextRenderer::setSize(const float size)
	{
		fonsSetSize(m_context, size);
	}

	void TextRenderer::drawText(const std::string& text, const math::vec2& position)
	{
		fonsDrawText(m_context, position.x, position.y, text.c_str(), NULL);
	}

	void TextRenderer::drawText(const Font& font, const std::string& text, const math::vec2& position, const Color& color, const float size)
	{
		setFont(font);
		setSize(size);
		setColor(color);
		drawText(text, position);
	}
}