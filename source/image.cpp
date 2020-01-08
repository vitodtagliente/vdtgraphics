#include <vdtgraphics/image.h>
#define STB_IMAGE_IMPLEMENTATION
#include <vdtgraphics/stb/stb_image.h>

namespace graphics
{
	Image::Image(const std::string& filename)
		: m_data()
		, m_width()
		, m_height()
		, m_channels()
	{
		//stbi_seflip_vertically_on_load(1);
		m_data = stbi_load(filename.c_str(), &m_width, &m_height, &m_channels, 4);
	}

	Image::~Image()
	{
		stbi_image_free(m_data);
	}

	unsigned char* Image::data() const
	{
		return m_data;
	}

	int Image::getWidth() const
	{
		return m_width;
	}

	int Image::getHeight() const
	{
		return m_height;
	}

	int Image::getChannels() const
	{
		return m_channels;
	}
}