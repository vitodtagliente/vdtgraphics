#include <vdtgraphics/api.h>
#include <vdtgraphics/platform.h>

#ifdef USE_OPENGL
#include <vdtgraphics/api/opengl/graphics_gl.h>
#endif 

#include <vdtgraphics/graphic_resource.h>
#include <vdtgraphics/shader_program.h>

namespace graphics
{
	API* const API::Factory::get()
	{
		return get(s_platformDefaultType);
	}

	API* const API::Factory::get(const Type type)
	{
		if (std::find(s_availableTypes.begin(), s_availableTypes.end(), type) != s_availableTypes.end())
		{
			// is this api already cached?
			const auto it = s_apis.find(type);
			if (it != s_apis.end())
			{
				return it->second;
			}

			// create and cache the new requested api and cache
			switch (type)
			{
#ifdef USE_OPENGL
			case API::Type::OpenGL:
			{
				API* const new_api = new API_GL();
				s_apis.insert({ type, new_api });
				return new_api;
			}
			break;
#endif 
			case API::Type::Null:
			default:
				return nullptr;
			break;
			}
		}
		return nullptr;
	}

	const std::vector<API::Type>& API::Factory::getAvailableTypes()
	{
		return s_availableTypes;
	}

	std::map<API::Type, API*> API::Factory::s_apis{};

	std::vector<API::Type> API::Factory::s_availableTypes{
		API::Type::Null
#ifdef USE_OPENGL
		, API::Type::OpenGL
#endif
	};

	API::Type API::Factory::s_platformDefaultType =
#ifdef USE_OPENGL
		API::Type::OpenGL
#else 
		GraphicsAPI::Type::Null
#endif
		;
	bool API::startup()
	{
		return initialize();
	}

	void API::update()
	{
		ResourcePool::getInstance().refresh();
	}

	void API::shutdown()
	{
		ResourcePool::getInstance().clear();
	}
	
	bool API::initialize()
	{
		for (const auto& pair : getDefaultShaderSources())
		{
			ShaderProgram* const program = ShaderProgram::parse(this, pair.second);
			if (program)
			{
				Material* material = new Material(program);
				m_materialLibrary.add(pair.first, material);
			}
		}
		return true;
	}
}