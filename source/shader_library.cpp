#include <vdtgraphics/shader_library.h>

#include <vdtgraphics/api.h>
#include <vdtgraphics/shader_program.h>

namespace graphics
{
	ShaderLibrary::ShaderLibrary(API* const api)
		: m_api(api)
		, m_programs()
	{
	}

	ShaderLibrary::~ShaderLibrary()
	{
		clear();
	}

	void ShaderLibrary::add(const std::string& name, ShaderProgram* const program)
	{
		m_programs.insert({ name, program });
	}

	ShaderProgram* const ShaderLibrary::get(const std::string& name) const
	{
		return m_programs.find(name)->second;
	}

	void ShaderLibrary::clear()
	{
		freeResources(m_programs);
	}
	
	void ShaderLibrary::initialize()
	{
		for (const std::pair<std::string, std::string>& shader : getDefaultShaderSources())
		{
			std::map<Shader::Type, std::string> sources;
			Shader::Reader::parse(shader.second, sources);
			// create shaders
			auto vertex = m_api->createShader(Shader::Type::Vertex, sources[Shader::Type::Vertex]);
			auto fragment = m_api->createShader(Shader::Type::Fragment, sources[Shader::Type::Fragment]);
			ShaderProgram* const program = m_api->createShaderProgram({ vertex, fragment });

			// free shaders
			delete vertex;
			delete fragment;

			if (program->getState() == ShaderProgram::State::Linked)
			{
				m_programs.insert({ shader.first, program });
			}
			else
			{
				delete program;
			}
		}
	}
}