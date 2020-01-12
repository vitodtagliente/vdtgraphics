/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <map>
#include <string>
#include <vector>
#include "shader.h"

namespace graphics
{
	class GraphicsAPI;
	class ShaderProgram;

	class ShaderLibrary
	{
	public:

		ShaderLibrary(GraphicsAPI* const api);
		virtual ~ShaderLibrary();

		void add(const std::string& name, ShaderProgram* const program);
		ShaderProgram* const get(const std::string& name) const;

		void clear();

		void initialize();

	protected:

		virtual const std::map<std::string, std::string>& getDefaultShaderSources() const = 0;

	private:

		template <typename T>
		void freeResources(std::map<std::string, T>& map)
		{
			for (auto it = map.begin(); it != map.end(); ++it)
			{
				delete it->second;
			}
			map.clear();
		}

		GraphicsAPI* m_api;
		std::map<std::string, ShaderProgram*> m_programs;
	};
}