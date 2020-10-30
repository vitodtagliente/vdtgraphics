#include <vdtgraphics/buffer.h>

namespace graphics
{

	BufferElement::BufferElement(
		const std::string& name, 
		const Type type, 
		const unsigned int count, 
		const std::size_t size,
		const bool normalized)
		: name(name)
		, type(type)
		, count(count)
		, size(size)
		, normalized(normalized)
	{
	}

	BufferLayout::BufferLayout()
		: m_elements()
		, m_stride()
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

	VertexBuffer::VertexBuffer(const std::size_t size)
		: m_type(Type::Dynamic)
		, m_size(size)
	{
	}

	VertexBuffer::VertexBuffer(const void* data, const std::size_t size)
		: m_type(Type::Static)
		, m_size(size)
	{
	}

	IndexBuffer::IndexBuffer(const unsigned int* indices, const std::size_t count)
		: m_count(count)
	{
	}
}