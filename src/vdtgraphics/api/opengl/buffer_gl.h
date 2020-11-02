/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/buffer.h>
#include "opengl.h"

namespace graphics
{
	class VertexBufferGL : public VertexBuffer
	{
	public:

		typedef unsigned int id_t;

		VertexBufferGL(const std::size_t vertexSize, const unsigned int vertices);
		VertexBufferGL(const std::size_t vertexSize, const unsigned int vertices, const UsageMode usage, const PrimitiveType primitiveType);
		virtual ~VertexBufferGL();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void update(const void* data, const unsigned int vertices) override;
		virtual void update(const void* data, const unsigned int vertices, const unsigned int offset) override;
		virtual void update(const BufferLayout& layout) override;

	protected:

		void activateLayout();

		id_t m_id;

	private:

		static GLenum getUsageModeGL(const VertexBuffer::UsageMode usage);
	};

	class IndexBufferGL final : public IndexBuffer
	{
	public:

		typedef unsigned int id_t;

		IndexBufferGL(const std::size_t size);
		IndexBufferGL(const unsigned int* indices, const std::size_t size, const BufferType type = BufferType::Static);
		virtual ~IndexBufferGL() override;

		virtual void bind() override;
		virtual void unbind() override;

		virtual void set(const unsigned int* indices, const std::size_t size) override;

	private:

		id_t m_id;
	};
}