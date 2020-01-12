/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <initializer_list>
#include <map>
#include <string>
#include <vector>

namespace graphics
{
	class Texture;

	class TextureLibrary
	{
	public:

		TextureLibrary();
		~TextureLibrary();

		void clear();

		// add a texture
		bool add(const std::string& name, Texture* const texture);
		// remove a texture
		bool remove(const std::string& name, const bool remove = true);

		// retrieve a texture
		Texture* const get(const std::string& name) const;

		// retrieve all textures
		const std::map<std::string, Texture*>& getTextures() const { return m_textures; }

	private:

		// cached textures
		std::map<std::string, Texture*> m_textures;

	};
}