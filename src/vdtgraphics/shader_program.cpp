#include <vdtgraphics/shader_program.h>

#include <map>
#include <vdtgraphics/api.h>
#include <vdtgraphics/shader.h>

namespace graphics
{
	ShaderProgram::ShaderProgram(const std::initializer_list<Shader*>& shaders)
		: m_id(INVALID_ID)
		, m_state(State::Unknown)
		, m_errorMessage()
	{
	}
	
	ShaderProgram* const ShaderProgram::parse(API* const api, const std::string& source)
	{
		std::map<Shader::Type, std::string> sources;
		Shader::Reader::parse(source, sources);
		// create shaders
		auto vertex = api->createShader(Shader::Type::Vertex, sources[Shader::Type::Vertex]);
		auto fragment = api->createShader(Shader::Type::Fragment, sources[Shader::Type::Fragment]);
		ShaderProgram* const program = api->createShaderProgram({ vertex, fragment });

		// free shaders
		delete vertex;
		delete fragment;

		if (program->getState() == ShaderProgram::State::Linked)
		{
			return program;
		}
		else
		{
			delete program;
			return nullptr;
		}
	}
}