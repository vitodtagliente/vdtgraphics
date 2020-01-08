/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/graphics_buffer.h>

namespace graphics
{
	class GraphicsBufferGL final : public GraphicsBuffer
	{
	public:

		GraphicsBufferGL(const Type type, const void* const data, const std::size_t size);
		virtual ~GraphicsBufferGL() override;

		virtual void bind() override;
		virtual void unbind() override;
	
		inline unsigned int id() const { return m_id; }

	private:

		// buffer id
		unsigned int m_id;
		// native buffer type
		unsigned int m_nativeType;
	};
}