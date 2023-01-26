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
			}
		}
		return m_state;
	}
}