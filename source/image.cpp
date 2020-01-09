#include <vdtgraphics/image.h>

namespace graphics
{
	Image::Image(unsigned char* const data, const int width, const int height, const int channels)
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