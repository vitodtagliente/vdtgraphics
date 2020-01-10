/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <string>
#include <vector>
#include <map>
#include <variant>
#include "color.h"
#include <vdtmath/matrix.h>
#include <vdtmath/vector.h>

using namespace math;

namespace graphics
{
	class ShaderProgram;
	class Texture;

	struct MaterialProperty
	{
		using value_t = std::variant<
			bool, float, int, 
			vec2, vec3, vec4, 
			mat2, mat3, mat4, 
			Texture*, Color
		>;

		enum class Type
		{
			Bool,
			Float,
			Int,
			Texture1D,
			Texture2D,
			Texture3D,
			TextureCube,
			Vec2,
			Vec3,
			Vec4,
			Mat2,
			Mat3,
			Mat4,
			Color
		};

		Type type;
		value_t value;
	};

	class Material
	{
	public:

		enum class Type
		{
			Default
		};

		Material(const Type type = Type::Default);
		Material(ShaderProgram * const shaderProgram, const Type type = Type::Default);
		Material(const Material& materal) = delete;
		~Material();

		void bind();
		void unbind();

		inline ShaderProgram * const getShaderProgram() const { return m_shaderProgram; }
		inline bool isInstance() const { return m_parent != nullptr; }
		inline Material* const getParent() const { return m_parent; }

		inline const std::map<std::string, MaterialProperty>& getProperties() const { return m_properties; }
		std::vector<MaterialProperty> getProperties(const MaterialProperty::Type type) const;

		void set(const std::string& name, const bool value);
		void set(const std::string& name, const int value);
		void set(const std::string& name, const float value);
		void set(const std::string& name, const vec2 value);
		void set(const std::string& name, const vec3 value);
		void set(const std::string& name, const vec4 value);
		void set(const std::string& name, const mat2 value);
		void set(const std::string& name, const mat3 value);
		void set(const std::string& name, const mat4 value);
		void set(const std::string& name, Texture* const value);
		void set(const std::string& name, const Color& value);

		// create a new instance of this material
		// example: many sprites that use differents uniforms parameters
		Material* const createInstance() const;

	private:

		// material type
		Type m_type;
		// shader program
		ShaderProgram* m_shaderProgram;
		// material attributes
		std::map<std::string, MaterialProperty> m_properties;
		// take track of all instances
		mutable std::vector<Material*> m_instances;
		// if it is an instance it shoud point to the parent material 
		Material* m_parent;
	};
}