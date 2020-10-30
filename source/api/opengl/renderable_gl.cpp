#include <vdtgraphics/api/opengl/renderable_gl.h>

#include <vector>
#include <vdtgraphics/api.h>
#include <vdtgraphics/api/opengl/opengl.h>
#include <vdtgraphics/api/opengl/buffer_gl.h>
#include <vdtgraphics/mesh.h>

namespace graphics
{
	RenderableGL::RenderableGL(API* const api, const Mesh& mesh)
		: Renderable(api, mesh)
		, m_vao()
	{
		initialize();
	}

	RenderableGL::RenderableGL(VertexBuffer* const vertexBuffer, IndexBuffer* const indexBuffer)
		: Renderable(vertexBuffer, indexBuffer)
		, m_vao()
	{
		initialize();
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

	void RenderableGL::initialize()
	{
		glGenVertexArrays(1, &m_vao);
		bind();

		m_vertexBuffer->bind();
		m_indexBuffer->bind();

		unsigned int elementIndex = 0;
		std::size_t elementSize = 0;
		for (const BufferElement& element : m_vertexBuffer->layout)
		{
			GLenum type = GL_FLOAT;
			switch (element.type)
			{
			case BufferElement::Type::Int: type = GL_INT; break;
			case BufferElement::Type::Bool: type = GL_BOOL; break;
			case BufferElement::Type::Float:
			default:
				type = GL_FLOAT; break;
			}

			glVertexAttribPointer(
				elementIndex, 
				element.count,
				type,
				element.normalized ? GL_TRUE : GL_FALSE, 
				static_cast<GLsizei>(m_vertexBuffer->layout.getStride()), 
				(void*)(elementSize)
			);
			elementSize += element.size;
			glEnableVertexAttribArray(elementIndex++);
		}

		unbind();
	}
}