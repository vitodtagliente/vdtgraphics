/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vector>
#include "render_command.h"

namespace graphics
{
	class CommandBuffer
	{
	public:

		CommandBuffer();
		~CommandBuffer();

		void sort();
		void push_back(const RenderCommand& command);
		inline const std::vector<RenderCommand>& getCommands() const { return m_commands; }
		void clear();

	private:
		
		// render commands
		std::vector<RenderCommand> m_commands;
	};
}