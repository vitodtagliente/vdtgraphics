/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/renderer.h>

namespace graphics
{
	class GraphicsModule;

	class RendererGL final : public Renderer
	{
	public:

		RendererGL(GraphicsAPI* const api);

		virtual void enableAlpha(const bool bEnabled = true) override;
		virtual void clear(const Color& color) override;
		virtual void draw(const unsigned int vertices = 3) override;
		virtual void drawIndexed(const unsigned int vertices) override;
		virtual void setViewport(const int width, const int height) override;
		virtual void setDrawingMode(const DrawingMode drawingMode) override;
		
	private:

		virtual const std::map<std::string, std::string>& getDefaultShaderSources() const override;
	};
}