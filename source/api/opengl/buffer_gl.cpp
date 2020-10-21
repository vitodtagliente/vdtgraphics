#include <vdtgraphics/api/opengl/buffer_gl.h>
#include <vdtgraphics/api/opengl/opengl.h>

namespace graphics
{
	BufferGL::BufferGL(const Type type, const void* const data, const std::size_t size)
		: Buffer(type, data, size)
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

		bind();
		glBufferData(m_nativeType, size, data, GL_STATIC_DRAW);
	}

	BufferGL::~BufferGL()
	{
		glDeleteBuffers(1, &m_id);
	}

	void BufferGL::bind()
	{
		glBindBuffer(m_nativeType, m_id);
	}

	void BufferGL::unbind()
	{
		glBindBuffer(m_nativeType, 0);
	}
}