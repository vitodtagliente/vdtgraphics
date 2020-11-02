#include <vdtgraphics/api/opengl/buffer_gl.h>

#include <vdtgraphics/api/opengl/opengl.h>

namespace graphics
{
	GLenum getUsageModeGL(const BufferUsageMode usage)
	{
		switch (usage)
		{
		case BufferUsageMode::Dynamic: return GL_DYNAMIC_DRAW;
		case BufferUsageMode::Static: return GL_STATIC_DRAW;
		case BufferUsageMode::Stream:
		default: return GL_STREAM_DRAW;
		}
	}

	VertexBufferGL::VertexBufferGL(const std::size_t vertexSize, const unsigned int vertices)
		: VertexBuffer(vertexSize, vertices)
	{
		glGenBuffers(1, &m_id);
		bind();
		glBufferData(GL_ARRAY_BUFFER, vertexSize * vertices, nullptr, getUsageModeGL(m_usage));
		unbind();
	}

	VertexBufferGL::VertexBufferGL(const std::size_t vertexSize, const unsigned int vertices, const BufferUsageMode usage, const PrimitiveType primitiveType)
		: VertexBuffer(vertexSize, vertices, usage, primitiveType)
	{
		glGenBuffers(1, &m_id);
		bind();
		glBufferData(GL_ARRAY_BUFFER, vertexSize * vertices, nullptr, getUsageModeGL(m_usage));
		unbind();
	}

	VertexBufferGL::~VertexBufferGL()
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBufferGL::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);

		activateLayout();
	}

	void VertexBufferGL::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBufferGL::update(const void* data, const unsigned int vertices)
	{
		update(data, vertices, 0);
	}

	void VertexBufferGL::update(const void* data, const unsigned int vertices, const unsigned int offset)
	{
		bind();
		if (vertices > m_vertices)
		{
			glBufferData(GL_ARRAY_BUFFER, m_vertexSize * vertices, nullptr, getUsageModeGL(m_usage));
			m_vertices = vertices;
		}
		glBufferSubData(GL_ARRAY_BUFFER, m_vertexSize * offset, m_vertexSize * vertices, data);
		unbind();
	}

	void VertexBufferGL::update(const BufferLayout& layout)
	{
		VertexBuffer::update(layout);		
	}

	void VertexBufferGL::activateLayout()
	{
		unsigned int elementIndex = 0;
		std::size_t elementSize = 0;
		for (const BufferElement& element : m_layout)
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
				static_cast<GLsizei>(m_layout.getStride()),
				(void*)(elementSize)
			);
			elementSize += element.size;
			glEnableVertexAttribArray(elementIndex++);
		}
	}

	const std::size_t IndexBufferGL::IndexSize{ sizeof(unsigned int) };

	IndexBufferGL::IndexBufferGL(const unsigned int indices)
		: IndexBuffer(indices)
	{
		glGenBuffers(1, &m_id);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize * indices, nullptr, getUsageModeGL(m_usage));
		unbind();
	}

	IndexBufferGL::IndexBufferGL(const unsigned int indices, const BufferUsageMode usage)
		: IndexBuffer(indices, usage)
	{
		glGenBuffers(1, &m_id);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize * indices, nullptr, getUsageModeGL(m_usage));
		unbind();
	}

	IndexBufferGL::~IndexBufferGL()
	{
		glDeleteBuffers(1, &m_id);
	}

	void IndexBufferGL::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBufferGL::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBufferGL::update(const void* data, const unsigned int indices)
	{
		update(data, indices, 0);
	}

	void IndexBufferGL::update(const void* data, const unsigned int indices, const unsigned int offset)
	{
		bind();
		if (indices > m_indices)
		{
			glBufferData(GL_ARRAY_BUFFER, IndexSize * indices, nullptr, getUsageModeGL(m_usage));
			m_indices = indices;
		}
		glBufferSubData(GL_ARRAY_BUFFER, IndexSize * offset, IndexSize * indices, data);
		unbind();
	}
}