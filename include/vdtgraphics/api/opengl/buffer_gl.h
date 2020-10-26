/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/buffer.h>

namespace graphics
{
	class VertexBufferGL : VertexBuffer
	{
	public:
		typedef unsigned int id_t;

		VertexBufferGL(const std::size_t size);
		VertexBufferGL(const void* data, const std::size_t size);
		virtual ~VertexBufferGL() override;

		virtual void bind() override;
		virtual void unbind() override;

		virtual void set(const void* data, const std::size_t size) override;

	protected:
		id_t m_id;
	};

	class IndexBufferGL final : public IndexBuffer
	{
	public:
		typedef unsigned int id_t;

		IndexBufferGL(unsigned int* indices, const std::size_t count);
		virtual ~IndexBufferGL() override;

		virtual void bind() override;
		virtual void unbind() override;

	private:
		id_t m_id;
	};
}