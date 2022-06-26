#include <vdtgraphics/font.h>

#include <fstream>
#include <string>
#include <sstream>

#define STB_TRUETYPE_IMPLEMENTATION
#include <vdtgraphics/stb_truetype.h>

namespace graphics
{
	Font::Font(const std::shared_ptr<uint8_t>& data)
		: data(data)
	{
	}

	std::optional<Font> Font::load(const std::filesystem::path& filename)
	{
		static const auto read = [](const std::filesystem::path& filename) -> std::string
		{
			std::ostringstream buf;
			std::ifstream input(filename.c_str());
			buf << input.rdbuf();
			return buf.str();
		};

		auto fontData = read(filename);
		std::shared_ptr<uint8_t> data = std::make_shared<uint8_t>(atlasWidth * atlasHeight);

		auto charInfo = std::make_unique<stbtt_packedchar[]>(charCount);

		stbtt_pack_context context;
		if (!stbtt_PackBegin(&context, data.get(), atlasWidth, atlasHeight, 0, 1, nullptr))
		{
			return std::nullopt;
		}

		stbtt_PackSetOversampling(&context, oversampleX, oversampleY);
		if (!stbtt_PackFontRange(&context, reinterpret_cast<const unsigned char*>(fontData.c_str()), 0, size, firstChar, charCount, charInfo.get()))
		{
			return std::nullopt;
		}

		stbtt_PackEnd(&context);

		return Font(data);
	}

	Font& Font::operator=(const Font& other)
	{
		data = other.data;
		return *this;
	}

	bool Font::operator==(const Font& other) const
	{
		return data == other.data;
	}

	bool Font::operator!=(const Font& other) const
	{
		return other != other.data;
	}
}