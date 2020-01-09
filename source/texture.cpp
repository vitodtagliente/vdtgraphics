#include <vdtgraphics/texture.h>

namespace graphics
{	
	Texture::Options::Options()
		: wrapS()
		, wrapT()
		, filterMin()
		, filterMax()
	{}

	Texture::Texture(const Image& image, const Options& options /*= Options{}*/)
		: m_id(INVALID_ID)
		, m_width(image.getWidth())
		, m_height(image.getHeight())
	{

	}
}