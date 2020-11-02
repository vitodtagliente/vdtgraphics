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

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement> list)
		: m_elements()
		, m_stride()
	{
		for (const auto& element : list)
		{
			push(element);
		}
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

	/*
	VertexBuffer::VertexBuffer(const std::size_t size)
		: m_layout()
		, m_count()
		, m_size(size)
	{
		m_layout.push(BufferElement("position", BufferElement::Type::Float, 0, sizeof(float) * 0));
	}

	VertexBuffer::VertexBuffer(const void* data, const unsigned int count, const std::size_t size, const BufferLayout& layout)
		: m_layout(layout)
		, m_count(count)
		, m_size(size)
	{
	}

	void VertexBuffer::set(const void* data, const unsigned int count, const std::size_t size)
	{
		m_count = count;
		set(data, size);
	}

	void VertexBuffer::set(const BufferLayout& layout)
	{
		m_layout = layout;
	}
	*/

	IndexBuffer::IndexBuffer(const std::size_t size)
		: m_type(BufferType::Dynamic)
		, m_size(size)
	{
	}

	IndexBuffer::IndexBuffer(const unsigned int* indices, const std::size_t size, const BufferType type)
		: m_type(type)
		, m_size(size)
	{
	}
}