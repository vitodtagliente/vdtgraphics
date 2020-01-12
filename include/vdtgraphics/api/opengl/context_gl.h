/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/context.h>

namespace graphics
{
	class ContextGL : public Context
	{
	public:

		ContextGL(GraphicsAPI* const api, const Type type = Type::Default);
		virtual ~ContextGL() override = default;

		virtual void enableAlpha(const bool bEnabled = true) override;
		virtual void clear(const Color& color) override;
		virtual void draw(const unsigned int vertices = 3) override;
		virtual void drawIndexed(const unsigned int numIndexes) override;
		virtual void setViewport(const int width, const int height) override;

	};
}