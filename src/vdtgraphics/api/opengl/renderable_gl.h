/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/renderable.h>

namespace graphics
{
	class RenderableGL final : public Renderable
	{
	public:

		RenderableGL(API* const api, const Mesh& mesh);
		RenderableGL(API* const api, const std::size_t vertices, const std::size_t indices);
		virtual ~RenderableGL() override;

		virtual void bind() override;
		virtual void unbind() override;

	private:

		void initialize(API* const api, const std::size_t vertices, const std::size_t indices);
		// vertex array object
		unsigned int m_vao;
	};
}