#include <vdtgraphics/renderable.h>

namespace graphics
{
	Renderable::Renderable()
		: m_mesh()
	{
	}

	Renderable::Renderable(const Mesh& t_mesh)
		: m_mesh(t_mesh)
	{
	}

	Renderable::~Renderable()
	{
	}
}