#include <vdtgraphics/api/opengl/shader_program_gl.h>
#include <vdtgraphics/api/opengl/opengl.h>
#include <vdtgraphics/shader.h>

namespace graphics
{
	ShaderProgramGL::ShaderProgramGL(const std::initializer_list<Shader*>& shaders)
		: ShaderProgram(shaders)
		, m_uniformLocations()
	{
		// create the program
		m_id = glCreateProgram();

		// attach shaders
		for (auto it = shaders.begin(); it != shaders.end(); ++it)
		{
			auto shader = *it;
			if (shader)
			{
				glAttachShader(m_id, shader->getId());
			}
		}

		// link the program
		glLinkProgram(m_id);

		int link_status{};
		glGetProgramiv(m_id, GL_LINK_STATUS, &link_status);
		if (link_status != GL_TRUE)
		{
			m_state = State::Error;
			// store the error message
			char log[1024];
			glGetProgramInfoLog(m_id, 1024, NULL, log);
			m_errorMessage = std::string{ log };

			// delete the program
			glDeleteProgram(m_id);
		}
		else
		{
			// detach shaders
			for (auto it = shaders.begin(); it != shaders.end(); ++it)
			{
				auto shader = *it;
				if (shader)
				{
					glDetachShader(m_id, shader->getId());
				}
			}
			m_state = State::Linked;
		}
	}

	ShaderProgramGL::~ShaderProgramGL()
	{
		glDeleteProgram(m_id);
	}

	void ShaderProgramGL::bind()
	{
		glUseProgram(m_id);
	}

	void ShaderProgramGL::unbind()
	{
		glUseProgram(0);
	}

	void ShaderProgramGL::set(const std::string& name, const bool value)
	{
		glUniform1i(getUniformLocation(name), static_cast<int>(value));
	}

	void ShaderProgramGL::set(const std::string& name, const int value)
	{
		glUniform1i(getUniformLocation(name), value);
	}

	void ShaderProgramGL::set(const std::string& name, const float value)
	{
		glUniform1f(getUniformLocation(name), value);
	}

	void ShaderProgramGL::set(const std::string& name, const float* const matrix)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix);
	}

	void ShaderProgramGL::set(const std::string& name, const std::vector<int>& value)
	{
		glUniform1iv(getUniformLocation(name), value.size(), &value[0]);
	}

	void ShaderProgramGL::set(const std::string& name, const float f1, const float f2, const float f3, const float f4)
	{
		glUniform4f(getUniformLocation(name), f1, f2, f3, f4);
	}

	int ShaderProgramGL::getUniformLocation(const std::string& name) const
	{
		if (m_uniformLocations.find(name) != m_uniformLocations.end())
		{
			return m_uniformLocations[name];
		}

		int location = glGetUniformLocation(m_id, name.c_str());
		if (location >= 0)
		{
			m_uniformLocations[name] = location;
		}
		return location;
	}
}