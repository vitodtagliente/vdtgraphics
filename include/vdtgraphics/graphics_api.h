/// Copyright (c) Vito Domenico Tagliente

#pragma once 

#include <map>
#include <initializer_list>
#include <string>
#include <vector>

#include "color.h"
#include "context.h"
#include "image.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

namespace graphics
{
	class Renderable;
	class Renderer2D;
	class Renderer3D;
	class ShaderProgram;

	class GraphicsAPI
	{
	public:

		enum class Type
		{
			Null,
			OpenGL
		};

		class Factory
		{
		public:

			static GraphicsAPI* const get();
			static GraphicsAPI* const get(const Type type);
			static const std::vector<Type>& getAvailableTypes();

		private:

			static std::map<Type, GraphicsAPI*> s_apis;
			static std::vector<Type> s_availableTypes;
			static GraphicsAPI::Type s_platformDefaultType;
		};

		GraphicsAPI(const Type type)
			: m_type(type)
		{

		}
		~GraphicsAPI() = default;

		virtual bool startup() = 0;
		virtual void shutdown() = 0;

		inline Type getType() const { return m_type; }

		// graphics objects creation
		virtual Context* const createContext(const Context::Type type = Context::Type::Default) = 0;
		virtual Renderable* createRenderable(const Mesh& mesh) = 0;
		virtual Renderer2D* const createRenderer2D(Context* const context) const;
		virtual Renderer3D* const createRenderer3D(Context* const context) const;
		virtual Shader* createShader(const Shader::Type type, const std::string& source) const = 0;
		virtual ShaderProgram* createShaderProgram(const std::initializer_list<Shader*>& shaders) const = 0;
		virtual Texture* createTexture(const Image& image, const Texture::Options& options = Texture::Options{}) const = 0;

	private:

		// api type
		Type m_type;
	};
}