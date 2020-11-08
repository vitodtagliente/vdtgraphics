#include <vdtgraphics/texture.h>

namespace graphics
{	
	const Texture::Coords Texture::Coords::Default{};

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

	Texture::Coords::Coords()
		: m_min()
		, m_max(1.0f, 1.0f)
	{
	}

	Texture::Coords::Coords(const vector2& min, const vector2& max)
		: m_min(std::max(0.0f, min.x), std::max(0.0f, min.y))
		, m_max(std::min(1.0f, max.x), std::min(1.0f, max.y))
	{
	}
}