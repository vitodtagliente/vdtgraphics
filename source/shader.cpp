#include <vdtgraphics/shader.h>

#include <fstream>
#include <sstream>

namespace graphics
{
	bool Shader::Reader::load(const std::string& filename, std::string& source)
	{
		std::string line;
		source.clear();
		std::ifstream file;
		file.open(filename.c_str(), std::ifstream::in);
		if (file.is_open())
		{
			while (std::getline(file, line))
			{
				source += line + end_line;
			}
			file.close();
			return true;
		}
		return false;
	}

	bool Shader::Reader::parse(const std::string& content, std::map<Type, std::string>& sources)
	{
		std::string line;
		sources.clear();
		std::istringstream stream(content);
		if (!std::empty(content))
		{
			Type currenshader_type;
			bool firsfound{ false };
			std::string source{};
			while (std::getline(stream, line))
			{
				if (line.find(shader_section) != std::string::npos)
				{
					if (firsfound)
					{
						sources[currenshader_type] = source;
					}
					else firsfound = true;

					if (line.find(to_string(Type::Vertex)) != std::string::npos)
						currenshader_type = Type::Vertex;
					else if (line.find(to_string(Type::Fragment)) != std::string::npos)
						currenshader_type = Type::Fragment;

					source.clear();
				}
				else source += line + end_line;
			}

			if (firsfound && !source.empty())
			{
				sources[currenshader_type] = source;
			}
			return true;
		}
		return false;
	}

	Shader::Shader(const Type type, const std::string& source)
		: m_id()
		, m_type(type)
		, m_state(State::Unknown)
		, m_errorMessage()
	{
	}
	
	std::string Shader::to_string(const Type type)
	{
		switch (type)
		{
		case Type::Vertex:
			return "vertex";
		case Type::Fragment:
		default:
			return "fragment";
		}
	}
	
	Shader::Type Shader::to_type(const std::string& str)
	{
		if (str == "fragment")
			return Type::Fragment;
		return Type::Vertex;
	}
}