/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/api.h>

namespace graphics
{
	class API_GL : public API
	{
	public:

		API_GL();
		virtual ~API_GL();

		virtual bool startup() override;
		virtual void shutdown() override;

		virtual IndexBuffer* const createIndexBuffer(const unsigned int* indices, const std::size_t count) override;
		virtual Renderable* const createRenderable(VertexBuffer* const vertexBuffer, IndexBuffer* const indexBuffer) override;
		virtual Renderable* const createRenderable(const Mesh& mesh) override;
		virtual Renderer2D* const createRenderer2D() override;
		virtual Renderer3D* const createRenderer3D() override;
		virtual Shader* const createShader(const Shader::Type type, const std::string& source) override;
		virtual ShaderProgram* const createShaderProgram(const std::initializer_list<Shader*>& shaders) override;
		virtual Texture* const createTexture(const Image& image, const Texture::Options& options = Texture::Options{}) override;
		virtual VertexBuffer* const createVertexBuffer(const void* data, const std::size_t size) override;

		virtual void enableAlpha(const bool enabled = true) override;
		virtual void clear(const Color& color) override;
		virtual void draw(const unsigned int vertices = 3) override;
		virtual void drawIndexed(const unsigned int numIndexes) override;
		virtual void setViewport(const int width, const int height) override;
		virtual unsigned int getTextureUnits() const override;

	private:

		virtual const std::map<std::string, std::string>& getDefaultShaderSources() const override;
	};
}