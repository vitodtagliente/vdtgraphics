/// Copyright (c) Vito Domenico Tagliente
#pragma once

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

	private:
		State m_state{ State::Default };
	};
}