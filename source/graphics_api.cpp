#include <vdtgraphics/graphics_api.h>
#include <vdtgraphics/platform.h>

#ifdef USE_OPENGL
#include <vdtgraphics/api/opengl/graphics_gl.h>
#endif 

namespace graphics
{
	GraphicsAPI* const GraphicsAPI::Factory::get()
	{
		return get(s_platformDefaultType);
	}

	GraphicsAPI* const GraphicsAPI::Factory::get(const Type type)
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
			case GraphicsAPI::Type::OpenGL:
			{
				GraphicsAPI* const new_api = new GraphicsAPI_GL();
				s_apis.insert({ type, new_api });
				return new_api;
			}
			break;
#endif 
			case GraphicsAPI::Type::Null:
			default:
				return nullptr;
			break;
			}
		}
		return nullptr;
	}

	const std::vector<GraphicsAPI::Type>& GraphicsAPI::Factory::getAvailableTypes()
	{
		return s_availableTypes;
	}

	std::map<GraphicsAPI::Type, GraphicsAPI*> GraphicsAPI::Factory::s_apis{};

	std::vector<GraphicsAPI::Type> GraphicsAPI::Factory::s_availableTypes{
		GraphicsAPI::Type::Null
#ifdef USE_OPENGL
		, GraphicsAPI::Type::OpenGL
#endif
	};

	GraphicsAPI::Type GraphicsAPI::Factory::s_platformDefaultType =
#ifdef USE_OPENGL
		GraphicsAPI::Type::OpenGL
#else 
		GraphicsAPI::Type::Null
#endif
		;
}