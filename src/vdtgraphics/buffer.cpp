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
}