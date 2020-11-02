/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/renderable.h>

namespace graphics
{
	class RenderableGL final : public Renderable
	{
	public:

		RenderableGL(VertexBuffer* const vertexBuffer, IndexBuffer* const indexBuffer);
		RenderableGL(API* const api, const Mesh& mesh);
		virtual ~RenderableGL() override;

		virtual void bind() override;
		virtual void unbind() override;
		
		virtual void update() override;

	private:

		// vertex array object
		unsigned int m_vao;
	};
}