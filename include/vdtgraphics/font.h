/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <array>
#include <filesystem>
#include <memory>
#include <optional>

#include "stb_truetype.h"

namespace graphics
{
	struct Font final
	{
		static const uint32_t size = 40;
		static const uint32_t atlasWidth = 1024;
		static const uint32_t atlasHeight = 1024;
		static const uint32_t oversampleX = 2;
		static const uint32_t oversampleY = 2;
		static const uint32_t firstChar = ' ';
		static const uint32_t charCount = '~' - ' ';
		static const uint8_t channels = 3;

		Font() = default;
		Font(const std::shared_ptr<uint8_t>& data);
		~Font() = default;

		static std::optional<Font> load(const std::filesystem::path& filename);

		Font& operator= (const Font& other);
		bool operator== (const Font& other) const;
		bool operator!= (const Font& other) const;

		std::shared_ptr<uint8_t> data;
	};
}