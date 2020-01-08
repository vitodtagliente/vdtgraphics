#include <vdtgraphics/api/opengl/renderer_gl.h>
#include <vdtgraphics/api/opengl/opengl.h>
#include <vdtgraphics/api/opengl/graphics_api_gl.h>

namespace graphics
{
	RendererGL::RendererGL(GraphicsAPI* const api)
		: Renderer(api)
	{
	}

	void RendererGL::enableAlpha(const bool bEnabled)
	{
		if (bEnabled)
		{
			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	void RendererGL::clear(const Color& color)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(color.red(), color.green(), color.blue(), color.alpha());
	}

	void RendererGL::draw(const unsigned int vertices)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices);
	}

	void RendererGL::drawIndexed(const unsigned int vertices)
	{
		glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, nullptr);
	}
	
	void RendererGL::setViewport(const int width, const int height)
	{
		glViewport(0, 0, width, height);
	}

	void RendererGL::setDrawingMode(const DrawingMode drawingMode)
	{
		switch (drawingMode)
		{
		case Renderer::DrawingMode::Lines:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case Renderer::DrawingMode::Fill:
		default:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}
	}
}