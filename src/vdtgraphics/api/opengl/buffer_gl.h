/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/buffer.h>

namespace graphics
{
	class VertexBufferGL : public VertexBuffer
	{
	public:

		typedef unsigned int id_t;

		VertexBufferGL(const std::size_t size);
		VertexBufferGL(const void* data, const unsigned int count, const std::size_t size, const BufferLayout& layout);
		virtual ~VertexBufferGL() override;

		virtual void bind() override;
		virtual void unbind() override;

		virtual void set(const void* data, const std::size_t size) override; 
		virtual void set(const BufferLayout& layout) override;

	protected:

		void registerLayout();

		id_t m_id;
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