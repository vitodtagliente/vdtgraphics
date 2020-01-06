#include <vdtgraphics/texture.h>

namespace graphics
{	
	Texture::Options::Options()
		: wrapS()
		, wrapT()
		, filterMin()
		, filterMax()
	{}

	Texture::Texture(const unsigned char* const data, const unsigned int width, const unsigned int height,
		const unsigned int channels, const Options& options /*= Options{}*/)
		: m_id(0)
		, m_width(width)
		, m_height(height)
		, m_format()
	{

	}
}