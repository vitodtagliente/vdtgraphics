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
		} context{};
	}

	Font::Font()
		: data()
		, path()
	{
	}

	Font::Font(const std::map<char, Glyph>& data, const std::filesystem::path& path)
		: data(data)
		, path(path)
	{
	}

	Font::Font(const Font& other)
		: data(other.data)
		, path(other.path)
	{
	}

	Font::~Font()
	{
	}

	Font Font::load(const std::filesystem::path& path)
	{
		if (!context.initialized)
		{
			context.ready = FT_Init_FreeType(&context.data) == 0;
		}

		if (!context.ready) return Font({}, path);

		FT_Face face;
		if (FT_New_Face(context.data, path.string().c_str(), 0, &face) != 0)
		{
			return Font({}, path);
		}

		FT_Set_Pixel_Sizes(face, 0, 48);
		std::map<char, Glyph> data;
		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
			{
				continue;
			}

			Texture::Options options;
			options.wrapS = GL_CLAMP_TO_EDGE;
			options.wrapT = GL_CLAMP_TO_EDGE;
			options.filterMin = GL_LINEAR;
			options.filterMax = GL_LINEAR;
			TexturePtr texture = std::make_shared<Texture>(face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, 3);
			
			Glyph glyph = {
				static_cast<unsigned int>(face->glyph->advance.x),
				math::vec2(static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top)),
				math::vec2(static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows)),
				std::move(texture)
			};
			data.insert(std::pair<char, Glyph>(c, glyph));
		}

		FT_Done_Face(face);

		return Font(data, path);
	}

	Font& Font::operator=(const Font& other)
	{
		data = other.data;
		path = other.path;
		return *this;
	}

	bool Font::operator==(const Font& other) const
	{
		return data == other.data && path == other.path;
	}

	bool Font::operator!=(const Font& other) const
	{
		return data != other.data || path != other.path;
	}
	
	Glyph& Glyph::operator=(const Glyph& other)
	{
		advance = other.advance;
		bearing = other.bearing;
		size = other.size;
		texture = other.texture;
		return *this;
	}

	bool Glyph::operator==(const Glyph& other) const
	{
		return advance == other.advance
			&& bearing == other.bearing
			&& size == other.size
			&& texture == other.texture;
	}

	bool Glyph::operator!=(const Glyph& other) const
	{
		return advance != other.advance
			|| bearing != other.bearing
			|| size != other.size
			|| texture != other.texture;
	}
}