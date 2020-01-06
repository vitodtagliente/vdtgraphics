#include <vdtgraphics/material_library.h>

#include <fstream>
#include <vdtgraphics/material.h>
#include <vdtgraphics/shader.h>
#include <vdtgraphics/shader_program.h>
#include <vdtgraphics/graphics_api.h>

namespace graphics
{
	MaterialLibrary::MaterialLibrary(GraphicsAPI* const api)
		: m_api(api)
		, m_materials()
		, m_shaders()
		, m_programs()
	{

	}

	MaterialLibrary::~MaterialLibrary()
	{
	}

	void MaterialLibrary::clear()
	{

	}

	Material* const MaterialLibrary::getMaterial(const std::string name) const
	{
		const auto it = m_materials.find(name);
		if (it != m_materials.end())
		{
			return it->second;
		}
		return nullptr;
	}

	Shader* const MaterialLibrary::getShader(const std::string& name) const
	{
		const auto it = m_shaders.find(name);
		if (it != m_shaders.end())
		{
			return it->second;
		}
		return nullptr;
	}

	ShaderProgram* const MaterialLibrary::getShaderProgram(const std::string& name) const
	{
		const auto it = m_programs.find(name);
		if (it != m_programs.end())
		{
			return it->second;
		}
		return nullptr;
	}

	unsigned int MaterialLibrary::loadMaterials(const std::string& path)
	{

		return 0;
	}

	unsigned int MaterialLibrary::loadShaders(const std::string& path)
	{
		return 0;
	}
	
	bool MaterialLibrary::add(const std::string& name, Material* const material)
	{
		const auto it = m_materials.find(name);
		if (it == m_materials.end())
		{
			m_materials.insert({ name, material });
			return true;
		}
		return false;
	}
	
	bool MaterialLibrary::add(const std::string& name, Shader* const shader)
	{
		const auto it = m_shaders.find(name);
		if (it == m_shaders.end())
		{
			m_shaders.insert({ name, shader });
			return true;
		}
		return false;
	}
	
	bool MaterialLibrary::add(const std::string& name, ShaderProgram* const program)
	{
		const auto it = m_programs.find(name);
		if (it == m_programs.end())
		{
			m_programs.insert({ name, program });
			return true;
		}
		return false;
	}
	
	Material* const MaterialLibrary::get(const std::string& name) const
	{
		const auto it = m_materials.find(name);
		if (it != m_materials.end())
		{
			return it->second;
		}
		return nullptr;
	}
}