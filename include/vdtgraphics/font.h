/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <string>

#include <vdtmath/vector2.h>
#include "texture_coords.h"

namespace graphics
{
	struct Font final
	{
		Font();
		Font(const std::shared_ptr<unsigned char>& data, const std::shared_ptr<void>& glyphInfo, const std::filesystem::path& path);
		Font(const Font& other);
		~Font();

		static Font load(const std::filesystem::path& filename);

		Font& operator= (const Font& other);
		bool operator== (const Font& other) const;
		bool operator!= (const Font& other) const;

		std::filesystem::path path;
		std::shared_ptr<unsigned char> data;
		std::shared_ptr<void> glyphInfo;

		static constexpr int width = 1024;
		static constexpr int height = 1024;

		static constexpr uint32_t oversampleX = 2;
		static constexpr uint32_t oversampleY = 2;
	};
}