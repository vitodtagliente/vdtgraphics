#include <vdtgraphics/font.h>

namespace graphics
{
	Font::Font()
		: data()
		, path()
		, size()
	{
	}

	Font::Font(const std::shared_ptr<unsigned char>& data, const std::filesystem::path& path, const size_t size)
		: data(data)
		, path(path)
		, size(size)
	{
	}

	Font::Font(const Font& other)
		: data(other.data)
		, path(other.path)
		, size(other.size)
	{
	}

	Font::~Font()
	{
	}

	Font Font::load(const std::filesystem::path& filename)
	{
		FILE* fp = 0;
		size_t dataSize = 0, readed;
		unsigned char* data = NULL;

		// Read in the font data.
		fp = fopen(filename.string().c_str(), "rb");
		if (fp == NULL) return Font();

		fseek(fp, 0, SEEK_END);
		dataSize = (int)ftell(fp);
		fseek(fp, 0, SEEK_SET);
		data = (unsigned char*)malloc(dataSize);
		if (data == NULL)
		{
			fclose(fp);
			return Font();
		}

		readed = fread(data, 1, dataSize, fp);
		fclose(fp);
		fp = 0;
		if (readed != dataSize)
		{
			free(data);
			if (fp) fclose(fp);
			return Font();
		}

		return Font(std::shared_ptr<unsigned char>(data), filename, dataSize);
	}

	Font& Font::operator=(const Font& other)
	{
		data = other.data;
		path = other.path;
		size = other.size;
		return *this;
	}

	bool Font::operator==(const Font& other) const
	{
		return data == other.data
			&& path == other.path
			&& size == other.size;
	}

	bool Font::operator!=(const Font& other) const
	{
		return data != other.data
			|| path != other.path
			|| size != other.size;
	}
}