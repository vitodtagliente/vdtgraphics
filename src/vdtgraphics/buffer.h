/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>
#include "graphic_resource.h"

namespace graphics
{
	struct BufferElement
	{
		enum class Type
		{
			None = 0,
			Float,
			Int,
			Bool
		};

		// the name of the element
		std::string name;
		// the type
		Type type;
		// num of components
		unsigned int count;
		// the size of the buffer element
		std::size_t size;
		// normalized
		bool normalized;

		BufferElement(
			const std::string& name,
			const Type type,
			const unsigned int count,
			const std::size_t size,
			const bool normalized = false
		);
	};

	class BufferLayout
	{
	public:

		BufferLayout();
		BufferLayout(const std::initializer_list<BufferElement> list);

		inline const std::vector<BufferElement>& getElements() const { return m_elements; }
		inline const std::size_t getStride() const { return m_stride; }
		void push(const BufferElement& element);
		void clear();

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

	private:

		// buffer elements
		std::vector<BufferElement> m_elements;
		std::size_t m_stride;
	};

	enum class BufferType
	{
		Dynamic,
		Static
	};

	class VertexBuffer : public GraphicResource<GraphicResourceType::VertexBuffer>
	{
	public:

		VertexBuffer(const std::size_t size);
		VertexBuffer(const void* data, const unsigned int count, const std::size_t size, const BufferLayout& layout);
		virtual ~VertexBuffer() = default;

		inline unsigned int getCount() const { return m_count; }
		inline const BufferLayout& getLayout() const { return m_layout; }
		inline std::size_t getSize() const { return m_size; }

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void set(const void* data, const unsigned int count, const std::size_t size);
		virtual void set(const void* data, const std::size_t size) = 0;
		virtual void set(const BufferLayout& layout);

	protected:

		// layout
		BufferLayout m_layout;
		// count of vertices
		unsigned int m_count;
		// size
		std::size_t m_size;
	};

	class IndexBuffer : public GraphicResource<GraphicResourceType::IndexBuffer>
	{
	public:

		IndexBuffer(const std::size_t size);
		IndexBuffer(const unsigned int* indices, const std::size_t size, const BufferType type = BufferType::Static);
		virtual ~IndexBuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		inline BufferType getType() const { return m_type; }
		inline std::size_t getSize() const { return m_size; }

		virtual void set(const unsigned int* indices, const std::size_t size) = 0;

	protected:

		// buffer type
		BufferType m_type;
		std::size_t m_size;
	};
}