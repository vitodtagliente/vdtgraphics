#include <vdtgraphics/texture.h>

namespace graphics
{	
	Texture::Options::Options()
		: wrapS(0)
		, wrapT(0)
		, filterMin(0)
		, filterMax(0)
	{}

	Texture::Texture(const Image& image, const Options& options /*= Options{}*/)
		: m_id(INVALID_ID)
		, m_width(image.getWidth())
		, m_height(image.getHeight())
		, m_assetName(image.getAssetName())
	{

	}
	
	Texture::~Texture()
	{

	}
}