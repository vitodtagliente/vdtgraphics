/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <map>
#include <string>

#include <vdtmath/vector2.h>

#include "texture.h"
#include "texture_rect.h"

namespace graphics
{
	struct Glyph final
	{
		// offset to advance to next glyph
		float advance;
		// offset from baseline to left/top of glyph
		math::vec2 bearing;
		// the texture rect
		TextureRect rect;
		// the size of the glyph
		math::vec2 size;

		Glyph& operator= (const Glyph& other);
		bool operator== (const Glyph& other) const;
		bool operator!= (const Glyph& other) const;
	};

	class Font final
	{
	public:
		Font();
		Font(TexturePtr texture, const std::map<char, Glyph>& data, const std::filesystem::path& path);
		Font(const Font& other);
		~Font();

		static Font load(const std::filesystem::path& filename, std::size_t size = 16);

		inline bool isValid() const { return !data.empty(); }

		Font& operator= (const Font& other);
		bool operator== (const Font& other) const;
		bool operator!= (const Font& other) const;

		std::map<char, Glyph> data;
		std::filesystem::path path;
		TexturePtr texture;

	private:
		static constexpr size_t num_glyphs = 128;
	};
}