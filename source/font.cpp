#include <vdtgraphics/font.h>

#include <fstream>
#include <string>
#include <sstream>

#define STB_TRUETYPE_IMPLEMENTATION
#include <vdtgraphics/stb_truetype.h>

namespace graphics
{
	const uint32_t size = 40;
	const uint32_t atlasWidth = 1024;
	const uint32_t atlasHeight = 1024;
	const uint32_t oversampleX = 2;
	const uint32_t oversampleY = 2;
	const uint32_t firstChar = ' ';
	const uint32_t charCount = '~' - ' ';

	Font::Font()
	{
	}

	Font::~Font()
	{
	}

	Font Font::load(const std::filesystem::path& filename)
	{
		static const auto read = [](const std::filesystem::path& filename) -> std::string
		{
			std::ostringstream buf;
			std::ifstream input(filename.c_str());
			buf << input.rdbuf();
			return buf.str();
		};

		Font font;

		// auto fontData = read(filename);
		// auto atlasData = std::make_unique<uint8_t[]>(atlasWidth * atlasHeight);
		// 
		// std::shared_ptr<stbtt_packedchar> data();
		// 
		// stbtt_pack_context context;
		// stbtt_PackBegin(&context, atlasData.get(), atlasWidth, atlasHeight, 0, 1, nullptr);
		// 
		// stbtt_PackSetOversampling(&context, oversampleX, oversampleY);
		// stbtt_PackFontRange(&context, reinterpret_cast<const unsigned char*>(fontData.c_str()), 0, size, firstChar, charCount, font.data.get());
		// 
		// stbtt_PackEnd(&context);

		return font;
	}

	Font& Font::operator=(const Font& other)
	{
		// // O: insert return statement here
		return *this;
	}

	bool Font::operator==(const Font& other) const
	{
		return false;
	}

	bool Font::operator!=(const Font& other) const
	{
		return false;
	}

}