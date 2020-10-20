#include <vdtgraphics/image.h>

namespace graphics
{
	Image::Image(unsigned char* const data, const unsigned int width, const unsigned int height, const unsigned int channels)
		: m_data(data)
		, m_width(width)
		, m_height(height)
		, m_channels(channels)
	{

	}

	Image::~Image()
	{
		std::free(m_data);
	}
}