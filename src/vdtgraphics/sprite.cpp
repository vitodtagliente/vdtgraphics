#include <vdtgraphics/sprite.h>

namespace graphics
{
	Sprite::Sprite()
		: texture(nullptr)
		, transform()
		, rect(0.0f, 0.0f, 1.0f, 1.0f)
	{

	}
	
	Sprite::Sprite(Texture* const texture, const math::transform& transform)
		: texture(texture)
		, transform(transform)
		, rect(0.0f, 0.0f, 1.0f, 1.0f)
	{
	}
	
	Sprite::Sprite(Texture* const texture, const math::transform& transform, const math::rect& rect)
		: texture(texture)
		, transform(transform)
		, rect(rect)
	{
	}
	
	Sprite::Sprite(const Sprite& sprite)
		: texture(sprite.texture)
		, transform(sprite.transform)
		, rect(sprite.rect)
	{
	}

	Sprite& Sprite::operator=(const Sprite& sprite)
	{
		texture = sprite.texture;
		transform = sprite.transform;
		rect = sprite.rect;

		return *this;
	}
}
