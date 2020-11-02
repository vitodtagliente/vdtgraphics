#include <vdtgraphics/api/opengl/renderable_gl.h>

#include <vector>
#include <vdtgraphics/api.h>
#include <vdtgraphics/api/opengl/opengl.h>
#include <vdtgraphics/api/opengl/buffer_gl.h>
#include <vdtgraphics/mesh.h>
#include <vdtgraphics/meshes/quad.h>

namespace graphics
{
	RenderableGL::RenderableGL(VertexBuffer* const vertexBuffer, IndexBuffer* const indexBuffer)
		: Renderable(vertexBuffer, indexBuffer)
		, m_vao()
	{
		glGenVertexArrays(1, &m_vao);
		update();
	}

	RenderableGL::RenderableGL(API* const api, const Mesh& mesh)
		: Renderable(api, mesh)
		, m_vao()
	{
		glGenVertexArrays(1, &m_vao);
		update();
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

	void RenderableGL::update()
	{
		bind();

		if (m_vertexBuffer != nullptr)
		{
			m_vertexBuffer->bind();
		}

		if (m_indexBuffer != nullptr)
		{
			m_indexBuffer->bind();
		}

		unbind();
	}
}