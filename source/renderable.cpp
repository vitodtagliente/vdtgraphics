#include <vdtgraphics/renderable.h>

#include <vdtgraphics/api.h>

namespace graphics
{
	Renderable::Renderable(const Mesh& mesh)
		: m_vertices(mesh.vertices.size())
		, m_indices(mesh.indices.size())
	{
	}

	Renderable::~Renderable()
	{
	}
	
	void Renderable::render(API* const api)
	{
		bind();
		if (m_indices != 0)
		{
			api->drawIndexed(m_indices);
		}
		else
		{
			api->draw(m_vertices);
		}
		unbind();
	}
}