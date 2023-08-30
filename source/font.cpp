#include <vdtgraphics/font.h>

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
		, size()
		, texture()
	{
	}

	Font::Font(TexturePtr texture, const std::map<char, Glyph>& data, const std::filesystem::path& path, const std::size_t size)
		: data(data)
		, path(path)
		, size(size)
		, texture(texture)
	{
	}

	Font::Font(const Font& other)
		: data(other.data)
		, path(other.path)
		, size(other.size)
		, texture(other.texture)
	{
	}

	Font::~Font()
	{
	}

	Font Font::load(const std::filesystem::path& path, const std::size_t size)
	{
		static Context context;

		if (!context.initialized)
		{
			context.ready = FT_Init_FreeType(&context.data) == 0;
		}

		if (!context.ready
			|| !std::filesystem::exists(path))
		{
			return Font(nullptr, {}, path, 0);
		}

		FT_Face face;
		if (FT_New_Face(context.data, path.string().c_str(), 0, &face) != 0)
		{
			return Font(nullptr, {}, path, 0);
		}
		FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(size));

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
			return Font(nullptr, {}, path, 0);
		}

		Texture::Options options;
		options.filter = Texture::Options::Filter::Linear;
		options.repeat = Texture::Options::Repeat::Disabled;
		constexpr unsigned int channels = 4;
		TexturePtr texture = std::make_shared<Texture>(nullptr, atlas_width, atlas_height, channels, options);
		
		std::map<char, Glyph> data;

		int x_pos = 0;
		for (unsigned char c = 32; c < num_glyphs; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
			{
				continue;
			}
			
			FT_Bitmap& bitmap = face->glyph->bitmap;

			const unsigned int width = bitmap.width;
			const float width_f = static_cast<float>(width);
			const unsigned int height = bitmap.rows;
			const float height_f = static_cast<float>(height);
			const std::size_t buffer_size = width * height * 4;

			if (buffer_size == 0) {
				continue;
			}

			std::vector<unsigned char> buffer(buffer_size);

			unsigned char* src = bitmap.buffer;
			unsigned char* startOfLine = src;
			int dst = 0;

			for (unsigned int y = 0; y < height; ++y) {
				src = startOfLine;
				for (unsigned int x = 0; x < width; ++x) {
					auto value = *src;
					src++;

					for (std::size_t c = 0; c < channels; ++c)
					{
						buffer[dst++] = value;
					}
				}
				startOfLine += bitmap.pitch;
			}

			Glyph glyph = {
				// advance
				static_cast<float>(face->glyph->advance.x / 64),
				// bearing
				math::vec2(static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top)),
				// texture rect
				TextureRect(static_cast<float>(x_pos) / atlas_width, 0.f, width_f / atlas_width, height_f / atlas_height),
				// size
				math::vec2(width_f, height_f),
			};
			data.insert(std::pair<char, Glyph>(c, glyph));

			texture->fillSubData(x_pos, 0, width, height, &buffer[0]);
			x_pos += width;
		}

		FT_Done_Face(face);

		return Font(std::move(texture), data, path, size);
	}

	Font& Font::operator=(const Font& other)
	{
		data = other.data;
		path = other.path;
		size = other.size;
		texture = other.texture;
		return *this;
	}

	bool Font::operator==(const Font& other) const
	{
		return data == other.data && path == other.path && texture == other.texture && size == other.size;
	}

	bool Font::operator!=(const Font& other) const
	{
		return data != other.data || path != other.path || texture != other.texture || size != other.size;
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