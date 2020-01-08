/// Copyright (c) Vito Domenico Tagliente

#pragma once 

#include <initializer_list>
#include <string>

#include "color.h"
#include "mesh.h"
#include "renderable.h"
#include "shader.h"
#include "shader_program.h"
#include "texture.h"

namespace graphics
{
	class GraphicsAPI
	{
	public:

		enum class Type
		{
			Null,
			OpenGL
		};

		GraphicsAPI(const Type type)
			: m_type(type)
		{

		}
		~GraphicsAPI() = default;

		virtual bool startup() = 0;
		virtual void shutdown() = 0;

		inline Type getType() const { return m_type; }

		// low level api abstraction
		virtual void clear(const Color& color) = 0;
		virtual void setViewport(const unsigned int width, const unsigned int height) = 0;
		virtual void draw(const unsigned int vertices = 3) = 0;
		virtual void drawIndexed(const unsigned int indices) = 0;

		virtual void enableAlpha(const bool enabled = true) = 0;

		// graphics objects creation
		virtual Shader* createShader(const Shader::Type type, const std::string& source) const = 0;
		virtual ShaderProgram* createShaderProgram(const std::initializer_list<Shader*>& shaders) const = 0;
		virtual Texture* createTexture(const unsigned char* const data, const unsigned int width, const unsigned int height,
			const unsigned int components, const Texture::Options& options = Texture::Options{}) const = 0;
		virtual Renderable* createRenderable(const Mesh& mesh) = 0;

	private:

		// api type
		Type m_type;
	};
}