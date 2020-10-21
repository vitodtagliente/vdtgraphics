/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/buffer.h>

namespace graphics
{
	class BufferGL final : public Buffer
	{
	public:

		BufferGL(const Type type, const void* const data, const std::size_t size);
		virtual ~BufferGL() override;

		virtual void bind() override;
		virtual void unbind() override;

	private:

		// native buffer type
		unsigned int m_nativeType;
	};
}