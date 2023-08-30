#include "vdtgraphics/context.h"

#include <glad/glad.h>

namespace graphics
{
	Context::State graphics::Context::initialize()
	{
		if (m_state == State::Default)
		{
			m_state = gladLoadGL() ? State::Initialized : State::Error;
			if (m_state == State::Initialized)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_TEXTURE_2D);
			}
		}
		return m_state;
	}

	void Context::viewport(unsigned const int width, unsigned const int height)
	{
		glViewport(0, 0, width, height);
	}

	void Context::clear(const Color& color, bool clearColorBuffer, bool clearDepthBuffer)
	{
		glClearColor(color.red, color.green, color.blue, color.alpha);
		int mode = 0;
		if (clearColorBuffer) mode |= GL_COLOR_BUFFER_BIT;
		if (clearDepthBuffer) mode |= GL_DEPTH_BUFFER_BIT;
		glClear(mode);
	}
}