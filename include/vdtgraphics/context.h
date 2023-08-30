/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include "color.h"

namespace graphics
{
	class Context
	{
	public:
		enum class State
		{
			Default,
			Error,
			Initialized
		};

		State initialize();
		State getState() const { return m_state; }

		void viewport(unsigned int width, unsigned int height);
		void clear(const Color& color = Color::Black, bool clearColorBuffer = true, bool clearDepthBuffer = true);

	private:
		State m_state{ State::Default };
	};
}