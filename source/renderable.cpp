#include <vdtgraphics/renderable.h>

namespace graphics
{
	Renderable::Renderable()
		: m_mesh()
	{
	}

	Renderable::Renderable(const Mesh& mesh)
		: m_mesh(mesh)
	{
	}

	Renderable::~Renderable()
	{
	}
}