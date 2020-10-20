#include <vdtgraphics/api.h>
#include <vdtgraphics/platform.h>
#include <vdtgraphics/renderer_2d.h>
#include <vdtgraphics/renderer_3d.h>

#ifdef USE_OPENGL
#include <vdtgraphics/api/opengl/graphics_gl.h>
#endif 

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
				API* const new_api = new GraphicsAPI_GL();
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

	Renderer2D* const API::createRenderer2D(Context* const context) const
	{
		Renderer2D* const renderer =  new Renderer2D(context);
		renderer->initialize();
		return renderer;
	}

	Renderer3D* const API::createRenderer3D(Context* const context) const
	{
		Renderer3D* const renderer = new Renderer3D(context);
		renderer->initialize();
		return renderer;
	}
}