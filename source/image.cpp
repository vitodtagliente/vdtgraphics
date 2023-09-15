#include <vdtgraphics/image.h>
#define STB_IMAGE_IMPLEMENTATION
#include <vdtgraphics/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION 
#include <vdtgraphics/stb_image_write.h>

namespace graphics
{
	bool Image::flip_vertically = false;

	Image::Image()
		: data()
		, width()
		, height()
		, channels()
	{
	}

	Image::Image(const std::shared_ptr<unsigned char>& data, const int width, const int height, const int channels)
		: data(data)
		, width(width)
		, height(height)
		, channels(channels)
	{
	}

	Image::Image(const Image& other)
		: data(other.data)
		, width(other.width)
		, height(other.height)
		, channels(other.channels)
	{
	}

	Image::~Image()
	{

	}

	Image Image::load(const std::filesystem::path& path)
	{
		// stbi_set_flip_vertically_on_load(1);
		int width, height, channels;
		stbi_set_flip_vertically_on_load(flip_vertically);
		std::shared_ptr<unsigned char> data(stbi_load(path.string().c_str(), &width, &height, &channels, 0));
		return Image(data, width, height, channels);
	}

	void Image::save(const std::filesystem::path& path)
	{
		stbi_write_png(path.string().c_str(), width, height, channels, data.get(), 0);
	}

	Image& Image::operator=(const Image& other)
	{
		data = other.data;
		width = other.width;
		height = other.height;
		channels = other.channels;
		return *this;
	}

	bool Image::operator==(const Image& other) const
	{
		return data == other.data
			&& width == other.width
			&& height == other.height
			&& channels == other.channels;
	}

	bool Image::operator!=(const Image& other) const
	{
		return data != other.data
			|| width != other.width
			|| height != other.height
			|| channels != other.channels;
	}
}