#include <vdtgraphics/api/opengl/context_gl.h>
#include <vdtgraphics/api/opengl/opengl.h>

namespace graphics
{
	ContextGL::ContextGL(GraphicsAPI* const api, const Type type)
		: Context(api, type)
	{
	}

	void ContextGL::enableAlpha(const bool bEnabled)
	{
		if (bEnabled)
		{
			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	void ContextGL::clear(const Color& color)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
	}

	void ContextGL::draw(const unsigned int vertices)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices);
	}

	void ContextGL::drawIndexed(const unsigned int numIndexes)
	{
		glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, nullptr);
	}

	void ContextGL::setViewport(const int width, const int height)
	{
		glViewport(0, 0, width, height);
	}
}