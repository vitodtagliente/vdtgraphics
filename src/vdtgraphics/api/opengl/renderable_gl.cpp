#include <vdtgraphics/api/opengl/renderable_gl.h>

#include <vector>
#include <vdtgraphics/api.h>
#include <vdtgraphics/api/opengl/opengl.h>
#include <vdtgraphics/api/opengl/buffer_gl.h>
#include <vdtgraphics/mesh.h>
#include <vdtgraphics/meshes/quad.h>

namespace graphics
{
	RenderableGL::RenderableGL(API* const api, const Mesh& mesh)
		: Renderable(api, mesh)
		, m_vao()
	{
		initialize(api, mesh.vertices.size(), mesh.indices.size());
	}

	RenderableGL::RenderableGL(API* const api, const std::size_t vertices, const std::size_t indices)
		: Renderable(api, vertices, indices)
		, m_vao()
	{
		initialize(api, vertices, indices);
	}

	RenderableGL::~RenderableGL()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void RenderableGL::bind()
	{
		glBindVertexArray(m_vao);
	}

	void RenderableGL::unbind()
	{
		glBindVertexArray(0);
	}

	void RenderableGL::initialize(API* const api, const std::size_t vertices, const std::size_t indices)
	{
		glGenVertexArrays(1, &m_vao);
		bind();

		if (m_vertexBuffer == nullptr)
		{
			m_vertexBuffer = api->createVertexBuffer(vertices * sizeof(float));
		}
		m_vertexBuffer->bind();
		
		if (m_indexBuffer == nullptr)
		{
			m_indexBuffer = api->createIndexBuffer(indices);
		}
		m_indexBuffer->bind();

		unbind();
	}
}