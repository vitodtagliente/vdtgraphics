#include <vdtgraphics/command_buffer.h>

namespace graphics
{
	CommandBuffer::CommandBuffer()
		: m_commands()
	{
	}

	CommandBuffer::~CommandBuffer()
	{
	}

	void CommandBuffer::sort()
	{
		// to be implemented
	}

	void CommandBuffer::push_back(const RenderCommand& command)
	{
		m_commands.push_back(command);
	}
	
	void CommandBuffer::clear()
	{
		m_commands.clear();
	}
}