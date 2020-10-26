#include <vdtgraphics/api/opengl/buffer_gl.h>

#include <vdtgraphics/api/opengl/opengl.h>

namespace graphics
{
	BufferGL::BufferGL(const Type type, const Mode mode)
		: Buffer(type, mode)
		, m_nativeType()
	{
		glGenBuffers(1, &m_id);

		switch (type)
		{
		case Buffer::Type::Index:
			m_nativeType = GL_ELEMENT_ARRAY_BUFFER;
			break;
		default:
		case Buffer::Type::Vertex:
			m_nativeType = GL_ARRAY_BUFFER;
			break;
		}
	}

	BufferGL::~BufferGL()
	{
		glDeleteBuffers(1, &m_id);
	}

	void BufferGL::fill(const void* data, const std::size_t dataSize, const BufferLayout& layout)
	{
		bind();
		glBufferData(m_nativeType, dataSize, data, GL_STATIC_DRAW);

		unsigned int elementIndex = 0;
		std::size_t size = 0;
		for (const BufferElement& element : layout.getElements())
		{
			glVertexAttribPointer(
				elementIndex, 
				element.count, 
				GL_FLOAT, 
				element.normalized ? GL_TRUE : GL_FALSE, 
				layout.getStride(), 
				(void*)size
			);
			glEnableVertexAttribArray(elementIndex++);
			size += element.size;
		}

		unbind();
	}

	void BufferGL::bind()
	{
		glBindBuffer(m_nativeType, m_id);
	}

	void BufferGL::unbind()
	{
		glBindBuffer(m_nativeType, 0);
	}
	
	IndexBufferGL::IndexBufferGL(unsigned int* indices, const std::size_t count)
		: IndexBuffer(indices, count)
		, m_id()
	{
		glGenBuffers(1, &m_id);

		bind();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER
			, count * sizeof(unsigned int)
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
}