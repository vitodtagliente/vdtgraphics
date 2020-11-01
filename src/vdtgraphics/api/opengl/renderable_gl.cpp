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

			m_vertexBuffer->layout.push(BufferElement("position", BufferElement::Type::Float, 3, sizeof(float) * 3));
			m_vertexBuffer->layout.push(BufferElement("color", BufferElement::Type::Float, 4, sizeof(float) * 4));
			m_vertexBuffer->layout.push(BufferElement("texture_coords", BufferElement::Type::Float, 2, sizeof(float) * 2));
			m_vertexBuffer->layout.push(BufferElement("texture_index", BufferElement::Type::Float, 1, sizeof(float) * 1));
		}
		m_vertexBuffer->bind();
		
		if (m_indexBuffer == nullptr)
		{
			m_indexBuffer = api->createIndexBuffer(indices);
		}
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