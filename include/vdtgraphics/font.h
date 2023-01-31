/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <map>
#include <string>

#include <vdtmath/vector2.h>

#include "texture.h"

namespace graphics
{
	struct Glyph final
	{
		// offset to advance to next glyph
		unsigned int advance;
		// offset from baseline to left/top of glyph
		math::vec2 bearing;
		// the size of the glyph
		math::vec2 size;
		TexturePtr texture;

		Glyph& operator= (const Glyph& other);
		bool operator== (const Glyph& other) const;
		bool operator!= (const Glyph& other) const;
	};

	class Font final
	{
	public:
		Font();
		Font(const std::map<char, Glyph>& data, const std::filesystem::path& path);
		Font(const Font& other);
		~Font();

		static Font load(const std::filesystem::path& filename);

		inline bool isValid() const { return !data.empty(); }

		Font& operator= (const Font& other);
		bool operator== (const Font& other) const;
		bool operator!= (const Font& other) const;

		std::filesystem::path path;
		std::map<char, Glyph> data;

	private:
		static constexpr size_t num_glyphs = 128;
	};
}