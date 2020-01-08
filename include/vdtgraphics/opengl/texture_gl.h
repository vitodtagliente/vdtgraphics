/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/texture.h>

namespace graphics
{
	class TextureGL final : public Texture
	{
	public:

		TextureGL(const unsigned char* const data, const unsigned int width, const unsigned int height,
			const unsigned int channels, const Options& options = Options{});
		~TextureGL();

		virtual void bind() override;
		void bind(const unsigned int slot);
		virtual void unbind() override;
	};
}