#include <vdtgraphics/font.h>

#include <fstream>

#define STB_TRUETYPE_IMPLEMENTATION 
#include <vdtgraphics/stb_truetype.h>

namespace graphics
{
	Font::Font()
		: data()
		, glyphInfo()
		, path()
	{
	}

	Font::Font(const std::shared_ptr<unsigned char>& data, const std::shared_ptr<void>& glyphInfo, const std::filesystem::path& path)
		: data(data)
		, glyphInfo(glyphInfo)
		, path(path)
	{
	}

	Font::Font(const Font& other)
		: data(other.data)
		, glyphInfo(other.glyphInfo)
		, path(other.path)
	{
	}

	Font::~Font()
	{
	}

	Font Font::load(const std::filesystem::path& path)
	{
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		if (!file.is_open())
			return Font();

		const auto size = file.tellg();
		file.seekg(0, std::ios::beg); 
		auto bytes = std::vector<unsigned char>(size);
		file.read(reinterpret_cast<char*>(&bytes[0]), size);
		file.close();

		unsigned char* data = new unsigned char[width * height]();
		
		static const uint32_t firstChar = ' ';
		static const uint32_t charCount = '~' - ' ';
		static const int fontSize = 40;
		stbtt_packedchar* charInfo = new stbtt_packedchar[charCount]();

		stbtt_pack_context context;
		if (!stbtt_PackBegin(&context, data, width, height, 0, 1, nullptr))
			return Font();

		stbtt_PackSetOversampling(&context, oversampleX, oversampleY);
		if (!stbtt_PackFontRange(&context, &bytes[0], 0, fontSize, firstChar, charCount, charInfo))
			return Font();

		stbtt_PackEnd(&context);

		return Font(std::shared_ptr<unsigned char>(data), std::shared_ptr<void>(charInfo), path);
	}

	Font& Font::operator=(const Font& other)
	{
		data = other.data;
		glyphInfo = other.glyphInfo;
		path = other.path;
		return *this;
	}

	bool Font::operator==(const Font& other) const
	{
		return data == other.data
			&& glyphInfo == other.glyphInfo
			&& path == other.path;
	}

	bool Font::operator!=(const Font& other) const
	{
		return data != other.data
			|| glyphInfo != other.glyphInfo
			|| path != other.path;
	}
}