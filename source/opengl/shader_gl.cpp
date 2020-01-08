#include <vdtgraphics/opengl/shader_gl.h>
#include <vdtgraphics/opengl/opengl.h>

namespace graphics
{
	ShaderGL::ShaderGL(const Type type, const std::string& source)
		: Shader(type, source)
	{
		m_id = glCreateShader(type == Type::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

		const char* source_pointer = source.c_str();
		glShaderSource(m_id, 1, &source_pointer, NULL);
		glCompileShader(m_id);

		int compile_state{};
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &compile_state);
		if (compile_state != GL_TRUE)
		{
			m_state = State::Error;
			char log[1024];
			glGetShaderInfoLog(m_id, 1024, NULL, log);
			m_errorMessage = std::string{ log };
		}
		else m_state = State::Compiled;
	}

	ShaderGL::~ShaderGL()
	{
		glDeleteShader(m_id);
		m_state = State::Unloaded;
	}
}