#include <vdtgraphics/context.h>
#include <vdtgraphics/api.h>

namespace graphics
{
	Context::Context(API* const api, const Type type)
		: m_api(api)
		, m_type(type)
	{
	}
}