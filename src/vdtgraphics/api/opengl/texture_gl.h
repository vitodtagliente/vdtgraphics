/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/texture.h>

namespace graphics
{
	class TextureGL final : public Texture
	{
	public:

		TextureGL(const Image& image, const Options& options = Options{});
		~TextureGL();

		virtual void bind() override;
		virtual void bind(const unsigned int slot) override;
		virtual void unbind() override;
	};
}