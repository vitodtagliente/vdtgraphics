#include <vdtgraphics/graphics_buffer.h>

namespace graphics
{
	GraphicsBufferElement::GraphicsBufferElement(
		const unsigned int count,
		const Type type,
		const std::size_t size,
		const bool normalized
		)
		: count(count)
		, type(type)
		, normalized(normalized)
		, size(count * size)
	{
	}

	GraphicsBufferLayout::GraphicsBufferLayout()
		: m_elements()
		, m_stride(0)
	{

	}

	void GraphicsBufferLayout::push(const GraphicsBufferElement& element)
	{
		m_elements.push_back(element);
		m_stride += element.size;
	}

	void GraphicsBufferLayout::clear()
	{
		m_elements.clear();
	}

	GraphicsBuffer::GraphicsBuffer(const Type type, const void * const data, const std::size_t size)
		: layout()
		, m_type(type)
		, m_size(size)
	{
	}

	GraphicsBuffer::~GraphicsBuffer()
	{
	}
}