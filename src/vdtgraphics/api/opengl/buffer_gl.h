/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/buffer.h>
#include "opengl.h"

namespace graphics
{
	GLenum getUsageModeGL(const BufferUsageMode usage);

	class VertexBufferGL : public VertexBuffer
	{
	public:

		typedef unsigned int id_t;

		VertexBufferGL(const std::size_t vertexSize, const unsigned int vertices);
		VertexBufferGL(const std::size_t vertexSize, const unsigned int vertices, const BufferUsageMode usage, const PrimitiveType primitiveType);
		virtual ~VertexBufferGL() override;

		virtual void bind() override;
		virtual void unbind() override;

		virtual void update(const void* data, const unsigned int vertices) override;
		virtual void update(const void* data, const unsigned int vertices, const unsigned int offset) override;
		virtual void update(const BufferLayout& layout) override;

	protected:

		void activateLayout();

		id_t m_id;
	};

	class IndexBufferGL final : public IndexBuffer
	{
	public:

		typedef unsigned int id_t;

		IndexBufferGL(const unsigned int indices);
		IndexBufferGL(const unsigned int indices, const BufferUsageMode usage);
		virtual ~IndexBufferGL() override;

		virtual void bind() override;
		virtual void unbind() override;

		virtual void update(const void* data, const unsigned int indices) override;
		virtual void update(const void* data, const unsigned int indices, const unsigned int offset) override;

	private:

		static const std::size_t IndexSize;

		id_t m_id;
	};
}