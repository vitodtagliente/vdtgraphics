#include <vdtgraphics/api/opengl/graphics_buffer_gl.h>
#include <vdtgraphics/api/opengl/opengl.h>

namespace graphics
{
	GraphicsBufferGL::GraphicsBufferGL(const Type type, const void* const data, const std::size_t size)
		: GraphicsBuffer(type, data, size)
		, m_id()
		, m_nativeType()
	{
		glGenBuffers(1, &m_id);

		switch (type)
		{
		case GraphicsBuffer::Type::Index:
			m_nativeType = GL_ELEMENT_ARRAY_BUFFER;
			break;
		default:
		case GraphicsBuffer::Type::Vertex:
			m_nativeType = GL_ARRAY_BUFFER;
			break;
		}

		bind();
		glBufferData(m_nativeType, size, data, GL_STATIC_DRAW);
	}

	GraphicsBufferGL::~GraphicsBufferGL()
	{
		glDeleteBuffers(1, &m_id);
	}

	void GraphicsBufferGL::bind()
	{
		glBindBuffer(m_nativeType, m_id);
	}

	void GraphicsBufferGL::unbind()
	{
		glBindBuffer(m_nativeType, 0);
	}
}