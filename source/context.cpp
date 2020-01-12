#include <vdtgraphics/context.h>
#include <vdtgraphics/graphics_api.h>

namespace graphics
{
	Context::Context(GraphicsAPI* const api, const Type type)
		: m_api(api)
		, m_type(type)
	{
	}
}