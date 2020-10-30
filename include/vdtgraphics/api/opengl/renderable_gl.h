/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/renderable.h>

namespace graphics
{
	class RenderableGL final : public Renderable
	{
	public:

		RenderableGL(API* const api, const Mesh& mesh);
		RenderableGL(VertexBuffer* const vertexBuffer, IndexBuffer* const indexBuffer);
		virtual ~RenderableGL() override;

		virtual void bind() override;
		virtual void unbind() override;

	private:

		void initialize();
		// vertex array object
		unsigned int m_vao;
	};
}