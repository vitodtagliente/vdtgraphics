/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/buffer.h>

namespace graphics
{
	class BufferGL final : public Buffer
	{
	public:

		BufferGL(const Type type, const Mode mode = Mode::Static);
		virtual ~BufferGL() override;

		virtual void fill(const void* data, const std::size_t size, const BufferLayout& layout = {}) override;

		virtual void bind() override;
		virtual void unbind() override;

	private:

		unsigned int m_nativeType;
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