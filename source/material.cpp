#include <vdtgraphics/material.h>

#include <vdtgraphics/shader_program.h>
#include <vdtgraphics/texture.h>

namespace graphics
{
	const std::string Material::Default::Name::Color = "color";
	const std::string Material::Default::Name::Texture = "texture";
	const std::string Material::Default::Name::CroppedTexture = "cropped_texture";

	const std::string Material::Default::Property::ModelViewProjectionMatrix = "u_ModelViewProjectionMatrix";
	const std::string Material::Default::Property::ViewProjectionMatrix = "u_ViewProjectionMatrix";
	const std::string Material::Default::Property::ModelTransformMatrix = "u_Transform";
	const std::string Material::Default::Property::Color = "u_Color";
	const std::string Material::Default::Property::Texture = "u_Texture";
	const std::string Material::Default::Property::TextureCropping = "u_TextureCropping";

	Material::Material(const Type type /*= Type::Default*/)
		: m_type(type)
		, m_shaderProgram()
		, m_properties()
		, m_instances()
		, m_parent()
	{

	}

	Material::Material(ShaderProgram * const shaderProgram, const Type type /*= Type::Default*/)
		: m_type(type)
		, m_shaderProgram(shaderProgram)
		, m_properties()
		, m_instances()
		, m_parent()
	{

	}

	Material::~Material()
	{
		for (Material* const instance : m_instances)
		{
			delete instance;
		}
	}

	void Material::bind()
	{
		m_shaderProgram->bind();

		int textures_counter = 0;
		for (const auto& pair : m_properties)
		{
			if (pair.second.type == MaterialProperty::Type::Texture2D)
			{
				Texture* const texture = std::get<Texture*>(pair.second.value);
				if (texture)
				{
					texture->bind();
					m_shaderProgram->set(pair.first, textures_counter++);
				}
			}
			else if (pair.second.type == MaterialProperty::Type::Color)
			{
				const Color& color = std::get<Color>(pair.second.value);
				m_shaderProgram->set(pair.first, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
			}
			else if (pair.second.type == MaterialProperty::Type::Vec4)
			{
				const vec4& vec = std::get<vector4>(pair.second.value);
				m_shaderProgram->set(pair.first, vec.x, vec.y, vec.z, vec.w);
			}
			else if (pair.second.type == MaterialProperty::Type::Mat4)
			{
				const matrix4& matrix = std::get<matrix4>(pair.second.value);
				m_shaderProgram->set(pair.first, &matrix.data[0]);
			}
		}
	}

	void Material::unbind()
	{
		m_shaderProgram->unbind();
	}

	std::vector<MaterialProperty> Material::getProperties(const MaterialProperty::Type type) const
	{
		std::vector<MaterialProperty> result;

		for (const auto& pair : m_properties)
		{
			if (pair.second.type == type)
			{
				result.push_back(pair.second);
			}
		}

		return result;
	}

	void Material::set(const std::string& name, const bool value)
	{
		m_properties.insert({ name, {MaterialProperty::Type::Bool, value} });
	}

	void Material::set(const std::string& name, const int value)
	{
		m_properties.insert({ name, {MaterialProperty::Type::Int, value} });
	}

	void Material::set(const std::string& name, const float value)
	{
		m_properties.insert({ name, {MaterialProperty::Type::Float, value} });
	}

	void Material::set(const std::string& name, const vec2 value)
	{
		m_properties.insert({ name, {MaterialProperty::Type::Vec2, value} });
	}

	void Material::set(const std::string& name, const vec3 value)
	{
		m_properties.insert({ name, {MaterialProperty::Type::Vec3, value} });
	}

	void Material::set(const std::string& name, const vec4 value)
	{
		m_properties.insert({ name, {MaterialProperty::Type::Vec4, value} });
	}

	void Material::set(const std::string& name, const mat2 value)
	{
		m_properties.insert({ name, {MaterialProperty::Type::Mat2, value} });
	}

	void Material::set(const std::string& name, const mat3 value)
	{
		m_properties.insert({ name, {MaterialProperty::Type::Mat3, value} });
	}

	void Material::set(const std::string& name, const mat4 value)
	{
		m_properties.insert({ name, {MaterialProperty::Type::Mat4, value} });
	}

	void Material::set(const std::string& name, Texture* const value)
	{
		m_properties.insert({ name, {MaterialProperty::Type::Texture2D, value} });
	}

	void Material::set(const std::string& name, const Color& value)
	{
		m_properties.insert({ name, {MaterialProperty::Type::Color, value} });
	}

	Material* const Material::createInstance() const
	{
		Material* const new_instance = new Material(m_shaderProgram, m_type);
		new_instance->m_properties = m_properties;
		if (!isInstance())
		{
			m_instances.push_back(new_instance);
			new_instance->m_parent = const_cast<Material*>(this);
		}
		else
		{
			new_instance->m_parent = m_parent;
		}
		return new_instance;
	}
}