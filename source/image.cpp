#include <vdtgraphics/image.h>

namespace graphics
{
	Image::Image()
		: m_data()
		, m_width()
		, m_height()
		, m_channels()
	{
	}

	Image::Image(unsigned char* const data, const unsigned int width, const unsigned int height, const unsigned int channels)
		: m_data(data)
		, m_width(width)
		, m_height(height)
		, m_channels(channels)
	{

	}

	Image::Image(const Image& image)
		: m_data(image.m_data)
		, m_width(image.m_width)
		, m_height(image.m_height)
		, m_channels(image.m_channels)
	{
	}

	Image::~Image()
	{

	}

	Image& Image::operator=(const Image& image)
	{
		m_data = image.m_data;
		m_width = image.m_width;
		m_height = image.m_height;
		m_channels = image.m_channels;

		return *this;
	}
	
	void Image::free()
	{
		std::free(m_data);
	}
}