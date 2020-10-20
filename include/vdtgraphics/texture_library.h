/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "resource_library.h"
#include "texture.h"

namespace graphics
{
	class TextureLibrary : public ResourceLibrary<Texture>
	{
	public:

		TextureLibrary();
		~TextureLibrary() = default;

	};
}