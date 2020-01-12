#include <vdtgraphics/texture_library.h>

#include <fstream>
#include <vdtgraphics/image.h>
#include <vdtgraphics/texture.h>

namespace graphics
{
	TextureLibrary::TextureLibrary()
		: m_textures()
	{

	}

	TextureLibrary::~TextureLibrary()
	{

	}

	void TextureLibrary::clear()
	{

	}

	bool TextureLibrary::add(const std::string& name, Texture* const texture)
	{
		const auto it = m_textures.find(name);
		if (it == m_textures.end())
		{
			m_textures.insert({ name, texture });
			return true;
		}
		return false;
	}

	bool TextureLibrary::remove(const std::string& name, const bool remove /*= true*/)
	{
		const auto it = m_textures.find(name);
		if (it != m_textures.end())
		{
			if (remove)
			{
				delete it->second;
			}
			m_textures.erase(it);
			return true;
		}
		return false;
	}

	Texture* const TextureLibrary::get(const std::string& name) const
	{
		const auto it = m_textures.find(name);
		if (it != m_textures.end())
		{
			return it->second;
		}
		return nullptr;
	}

}