#include <vdtgraphics/api/opengl/buffer_gl.h>

#include <vdtgraphics/api/opengl/opengl.h>

namespace graphics
{
	VertexBufferGL::VertexBufferGL(const std::size_t size)
		: VertexBuffer(size)
	{
		glGenBuffers(1, &m_id);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		unbind();
	}

	VertexBufferGL::VertexBufferGL(const void* data, const std::size_t size, const BufferType type)
		: VertexBuffer(data, size, type)
	{
		glGenBuffers(1, &m_id);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		unbind();
	}

	VertexBufferGL::~VertexBufferGL()
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBufferGL::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBufferGL::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBufferGL::set(const void* data, const std::size_t size)
	{
		if (m_type == BufferType::Dynamic)
		{
			bind();
			glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
			unbind();
		}
	}
	
	IndexBufferGL::IndexBufferGL(const std::size_t size)
		: IndexBuffer(size)
		, m_id()
	{
		glGenBuffers(1, &m_id);

		bind();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER
			, size * sizeof(unsigned int)
			, nullptr
			, GL_DYNAMIC_DRAW);

		unbind();
	}

	IndexBufferGL::IndexBufferGL(const unsigned int* indices, const std::size_t size, const BufferType type)
		: IndexBuffer(indices, size, type)
		, m_id()
	{
		glGenBuffers(1, &m_id);

		bind();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER
			, size * sizeof(unsigned int)
			, indices
			, GL_STATIC_DRAW);

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
	
	void IndexBufferGL::set(const unsigned int* indices, const std::size_t size)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(unsigned int), indices);
		m_size = size;
	}
}