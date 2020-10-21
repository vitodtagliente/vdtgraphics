/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <cstddef>
#include <vector>

namespace graphics
{
	struct BufferElement
	{
		enum class Type
		{
			Char,
			Float,
			Integer,
			UnsignedInteger
		};

		// the number of elements
		unsigned int count;
		// the type
		Type type;
		// normalized
		bool normalized;
		// size
		std::size_t size;

		BufferElement(
			const unsigned int count,
			const Type type,
			const std::size_t size,
			const bool normalized = false
		);
	};

	class BufferLayout
	{
	public:

		BufferLayout();

		inline std::size_t getStride() const { return m_stride; }
		inline const std::vector<BufferElement>& getElements() const { return m_elements; }

		void push(const BufferElement& element);
		void clear();

	private:

		// buffer elements
		std::vector<BufferElement> m_elements;
		// layout stride
		std::size_t m_stride;
	};

	class Buffer
	{
	public:

		typedef unsigned int id_t;

		enum class Type
		{
			Vertex,
			Index
		};

		Buffer(const Type type, const void * const data, const std::size_t size);
		virtual ~Buffer();

		inline id_t getId() const { return m_id; }
		inline Type type() const { return m_type; }
		inline std::size_t size() const { return m_size; }

		virtual void bind() = 0;
		virtual void unbind() = 0;

		// buffer layout
		BufferLayout layout;

		static constexpr id_t INVALID_ID = 0;

	protected:

		// the id of the buffer
		id_t m_id;
		// buffer type
		Type m_type;
		// buffer size
		std::size_t m_size;

	};
}