/// Copyright (c) Vito Domenico Tagliente

#pragma once 

#include <initializer_list>
#include <map>
#include <string>
#include <vector>

#include "buffer.h"
#include "material.h"
#include "mesh.h"
#include "primitive_type.h"
#include "renderable.h"
#include "renderer_2d.h"
#include "renderer_3d.h"
#include "resource_library.h"
#include "shader.h"
#include "shader_program.h"
#include "texture.h"

namespace graphics
{
	class API
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

			static API* const get();
			static API* const get(const Type type);
			static const std::vector<Type>& getAvailableTypes();

		private:

			static std::map<Type, API*> s_apis;
			static std::vector<Type> s_availableTypes;
			static API::Type s_platformDefaultType;
		};

		API(const Type type)
			: m_type(type)
		{

		}
		API(const API&) = delete;
		virtual ~API() = default;

		virtual bool startup();
		virtual void update();
		virtual void shutdown();

		API& operator= (const API&) = delete;

		inline Type getType() const { return m_type; }
		const ResourceLibrary<Material>& getMaterialLibrary() const { return m_materialLibrary; }

		virtual IndexBuffer* const createIndexBuffer(const unsigned int* indices, const std::size_t size, const BufferType type = BufferType::Static) = 0;
		virtual IndexBuffer* const createIndexBuffer(const std::size_t size) = 0;
		virtual Renderable* const createRenderable(const std::size_t vertices, const std::size_t indices) = 0;
		virtual Renderable* const createRenderable(const Mesh& mesh) = 0;
		virtual Renderer2D* const createRenderer2D() = 0;
		virtual Renderer3D* const createRenderer3D() = 0;
		virtual Shader* const createShader(const Shader::Type type, const std::string& source) = 0;
		virtual ShaderProgram* const createShaderProgram(const std::initializer_list<Shader*>& shaders) = 0;
		virtual Texture* const createTexture(const Image& image, const Texture::Options& options = Texture::Options{}) = 0;
		virtual VertexBuffer* const createVertexBuffer(const std::size_t vertexSize, const unsigned int vertices) = 0;
		virtual VertexBuffer* const createVertexBuffer(const std::size_t vertexSize, const unsigned int vertices, const VertexBuffer::UsageMode usage, const PrimitiveType primitiveType) = 0;

		virtual void clear(const Color& color) = 0;
		virtual void draw(VertexBuffer* const vertexBuffer);
		virtual void draw(IndexBuffer* const indexBuffer);
		virtual void draw(const PrimitiveType primitiveType, const unsigned int vertices = 3) = 0;
		virtual void drawIndexed(const PrimitiveType primitiveType, const unsigned int numIndexes) = 0;
		virtual void enableAlpha(const bool enabled = true) = 0;
		virtual void setViewport(const int width, const int height) = 0;
		virtual unsigned int getTextureUnits() const = 0;

	protected:

		virtual bool initialize();
		virtual const std::map<std::string, std::string>& getDefaultShaderSources() const = 0;

		// api type
		Type m_type;
		// default meterial library
		ResourceLibrary<Material> m_materialLibrary;
	};
}