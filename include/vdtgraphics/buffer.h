/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <cstddef>
#include <string>
#include <vector>

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

	class VertexBuffer
	{
	public:

		enum class Type
		{
			Static,
			Dynamic
		};

		VertexBuffer(const std::size_t size);
		VertexBuffer(const void* data, const std::size_t size);
		virtual ~VertexBuffer() = default;

		inline Type getType() const { return m_type; }
		inline std::size_t getSize() const { return m_size; }

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void set(const void* data, const std::size_t size) = 0;

		BufferLayout layout;

	protected:
		// buffer type
		Type m_type;
		// size
		std::size_t m_size;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* indices, const std::size_t count);
		virtual ~IndexBuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		inline std::size_t getCount() const { return m_count; }

	private:
		std::size_t m_count;
	};
}