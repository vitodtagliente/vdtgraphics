/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <memory>

#include "stb_truetype.h"

namespace graphics
{
	struct Font final
	{
		Font();
		~Font();

		static Font load(const std::filesystem::path& filename);

		Font& operator= (const Font& other);
		bool operator== (const Font& other) const;
		bool operator!= (const Font& other) const;

		std::shared_ptr<stbtt_packedchar[]> data;
	};
}