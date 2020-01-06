/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <initializer_list>
#include <map>
#include <string>
#include <vector>

namespace graphics
{
	class GraphicsAPI;
	class Texture;

	class TextureLibrary
	{
	public:

		TextureLibrary(GraphicsAPI* api);
		~TextureLibrary();

		void clear();

		// load textures form path
		unsigned int load(const std::string& path);
		// load texture given filenames
		unsigned int load(const std::vector<std::string>& files);

		// add a texture
		Texture* const add(const std::string& name, const std::string& filename);
		bool add(const std::string& name, Texture* const texture);
		// remove a texture
		bool remove(const std::string& name, const bool remove = true);

		// retrieve a texture
		Texture* const get(const std::string& name) const;

		// retrieve all textures
		const std::map<std::string, Texture*>& getTextures() const { return m_textures; }

	private:

		// graphics api
		GraphicsAPI* m_api;
		// cached textures
		std::map<std::string, Texture*> m_textures;

	};
}