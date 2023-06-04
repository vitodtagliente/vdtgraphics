#include <vdtgraphics/font.h>

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H  

namespace graphics
{
	namespace
	{
		struct Context
		{
			FT_Library data;
			bool initialized = false;
			bool ready = false;
		};
	}

	Font::Font()
		: data()
		, path()
		, texture()
	{
	}

	Font::Font(TexturePtr texture, const std::map<char, Glyph>& data, const std::filesystem::path& path)
		: data(data)
		, path(path)
		, texture(texture)
	{
	}

	Font::Font(const Font& other)
		: data(other.data)
		, path(other.path)
		, texture(other.texture)
	{
	}

	Font::~Font()
	{
	}

	Font Font::load(const std::filesystem::path& path)
	{
		static Context context;

		if (!context.initialized)
		{
			context.ready = FT_Init_FreeType(&context.data) == 0;
		}

		if (!context.ready) return Font(nullptr, {}, path);

		FT_Face face;
		if (FT_New_Face(context.data, path.string().c_str(), 0, &face) != 0)
		{
			return Font(nullptr, {}, path);
		}
		FT_Set_Pixel_Sizes(face, 0, font_size);

		// determine the size of the atlas
		int atlas_width = 0;
		int atlas_height = 0;
		for (unsigned char c = 32; c < num_glyphs; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
			{
				continue;
			}

			atlas_width += face->glyph->bitmap.width;
			atlas_height = std::max(atlas_height, static_cast<int>(face->glyph->bitmap.rows));
		}

		if (atlas_width == 0 || atlas_height == 0)
		{
			return Font(nullptr, {}, path);
		}

		TexturePtr texture = std::make_shared<Texture>(nullptr, atlas_width, atlas_height, 1);

		std::map<char, Glyph> data;

		int x_pos = 0;
		for (unsigned char c = 32; c < num_glyphs; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
			{
				continue;
			}

			Glyph glyph = {
				// advance
				static_cast<float>(face->glyph->advance.x / 64) / font_size,
				// bearing
				math::vec2(static_cast<float>(face->glyph->bitmap_left) / font_size, static_cast<float>(face->glyph->bitmap_top) / font_size),
				// texture rect
				TextureRect(static_cast<float>(x_pos) / atlas_width, 0.f, static_cast<float>(face->glyph->bitmap.width) / atlas_width, 1.f),
				// size
				math::vec2(static_cast<float>(face->glyph->bitmap.width) / font_size, static_cast<float>(face->glyph->bitmap.rows) / font_size)
			};
			data.insert(std::pair<char, Glyph>(c, glyph));

			texture->fillSubData(x_pos, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
			x_pos += face->glyph->bitmap.width;
		}

		FT_Done_Face(face);

		return Font(std::move(texture), data, path);
	}

	Font& Font::operator=(const Font& other)
	{
		data = other.data;
		path = other.path;
		texture = other.texture;
		return *this;
	}

	bool Font::operator==(const Font& other) const
	{
		return data == other.data && path == other.path && texture == other.texture;
	}

	bool Font::operator!=(const Font& other) const
	{
		return data != other.data || path != other.path || texture != other.texture;
	}

	Glyph& Glyph::operator=(const Glyph& other)
	{
		advance = other.advance;
		bearing = other.bearing;
		rect = other.rect;
		size = other.size;
		return *this;
	}

	bool Glyph::operator==(const Glyph& other) const
	{
		return advance == other.advance
			&& bearing == other.bearing
			&& rect == other.rect
			&& size == other.size;
	}

	bool Glyph::operator!=(const Glyph& other) const
	{
		return advance != other.advance
			|| bearing != other.bearing
			|| rect != other.rect
			|| size != other.size;
	}
}