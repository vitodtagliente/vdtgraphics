#include <vdtgraphics/buffer.h>

namespace graphics
{
	BufferElement::BufferElement(
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

	BufferLayout::BufferLayout()
		: m_elements()
		, m_stride(0)
	{

	}

	void BufferLayout::push(const BufferElement& element)
	{
		m_elements.push_back(element);
		m_stride += element.size;
	}

	void BufferLayout::clear()
	{
		m_elements.clear();
	}

	Buffer::Buffer(const Type type, const void * const data, const std::size_t size)
		: layout()
		, m_type(type)
		, m_size(size)
	{
	}

	Buffer::~Buffer()
	{
	}
}