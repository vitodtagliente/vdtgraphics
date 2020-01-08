/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/renderable.h>

namespace graphics
{
	class RenderableGL final : public Renderable
	{
	public:

		RenderableGL();
		RenderableGL(const Mesh& mesh);
		virtual ~RenderableGL() override;

		virtual void bind() override;
		virtual void unbind() override;

	private:

		// vertex array object
		unsigned int m_vao;
		// vertex buffer object
		unsigned int m_vbo;
		// elements buffer object
		unsigned int m_ebo;
	};
}