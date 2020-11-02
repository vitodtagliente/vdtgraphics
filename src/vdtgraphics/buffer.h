/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>
#include "graphic_resource.h"
#include "primitive_type.h"

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

	enum class BufferUsageMode
	{
		Dynamic,
		Static,
		Stream
	};

	class VertexBuffer : public GraphicResource<GraphicResourceType::VertexBuffer>
	{
	public:

		VertexBuffer(const std::size_t vertexSize, const unsigned int vertices)
			: m_vertexSize(vertexSize)
			, m_usage(BufferUsageMode::Static)
			, m_primitiveType(PrimitiveType::Triangles)
			, m_vertices(vertices)
			, m_layout()
		{}
		VertexBuffer(const std::size_t vertexSize, const unsigned int vertices, const BufferUsageMode usage, const PrimitiveType primitiveType)
			: m_vertexSize(vertexSize)
			, m_usage(usage)
			, m_primitiveType(primitiveType)
			, m_vertices(vertices)
			, m_layout()
		{}
		virtual ~VertexBuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void update(const void* data, const unsigned int vertices) = 0;
		virtual void update(const void* data, const unsigned int vertices, const unsigned int offset) = 0;
		virtual void update(const BufferLayout& layout) { m_layout = layout; }

		inline std::size_t getVertexSize() const { return m_vertexSize; }
		inline BufferUsageMode getUsageMode() const { return m_usage; }
		inline PrimitiveType getPrimitiveType() const { return m_primitiveType; }
		inline unsigned int getVerticesCount() const { return m_vertices; }
		inline const BufferLayout& getLayout() const { return m_layout; }

	protected:

		std::size_t m_vertexSize;
		BufferUsageMode m_usage;
		PrimitiveType m_primitiveType;
		unsigned int m_vertices;
		BufferLayout m_layout;
	};

	class IndexBuffer : public GraphicResource<GraphicResourceType::IndexBuffer>
	{
	public:

		IndexBuffer(const unsigned int indices)
			: m_usage(BufferUsageMode::Static)
			, m_indices(indices)
		{}
		IndexBuffer(const unsigned int indices, const BufferUsageMode usage) 
			: m_usage(usage)
			, m_indices(indices)
		{}
		virtual ~IndexBuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		inline BufferUsageMode getUsageMode() const { return m_usage; }
		inline unsigned int getIndicesCount() const { return m_indices; }

		virtual void update(const void* data, const unsigned int indices) = 0;
		virtual void update(const void* data, const unsigned int indices, const unsigned int offset) = 0;

	protected:

		// buffer type
		BufferUsageMode m_usage;
		unsigned int m_indices;
	};
}