#include <vdtgraphics/shader_program.h>
#include <vdtgraphics/shader.h>

namespace graphics
{
	ShaderProgram::ShaderProgram(const std::initializer_list<Shader*>& shaders)
		: m_id(INVALID_ID)
		, m_state(State::Unknown)
		, m_errorMessage()
	{
	}
}