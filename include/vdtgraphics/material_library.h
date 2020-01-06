/// Copyright (c) Vito Domenico Tagliente

#pragma once 

#include <map>
#include <string>
#include <vector>

namespace graphics
{
	class Material;
	class Shader;
	class ShaderProgram;
	class GraphicsAPI;

	class MaterialLibrary final
	{
	public:

		MaterialLibrary(GraphicsAPI* const api);
		~MaterialLibrary();

		void clear();

		Material* const getMaterial(const std::string name) const;
		Shader* const getShader(const std::string& name) const;
		ShaderProgram* const getShaderProgram(const std::string& name) const;

		// load materials in folder
		unsigned int loadMaterials(const std::string& path);
		unsigned int loadShaders(const std::string& path);

		bool add(const std::string& name, Material* const material);
		bool add(const std::string& name, Shader* const shader);
		bool add(const std::string& name, ShaderProgram* const program);

		// retrieve a material
		Material* const get(const std::string& name) const;

		const std::map<std::string, Material*>& getMaterials() const { return m_materials; }
		const std::map<std::string, Shader*>& getShaders() const { return m_shaders; }
		const std::map<std::string, ShaderProgram*>& getPrograms() const { return m_programs; }

	private:

		GraphicsAPI* m_api;
		std::map<std::string, Material*> m_materials;
		std::map<std::string, Shader*> m_shaders;
		std::map<std::string, ShaderProgram*> m_programs;

	};
}