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

		virtual Renderable* const createRenderable(const Mesh& mesh) override;
		virtual Renderer2D* const createRenderer2D() override;
		virtual Renderer3D* const createRenderer3D() override;
		virtual Shader* const createShader(const Shader::Type type, const std::string& source) override;
		virtual ShaderProgram* const createShaderProgram(const std::initializer_list<Shader*>& shaders) override;
		virtual Texture* const createTexture(const Image& image, const Texture::Options& options = Texture::Options{}) override;

		virtual void enableAlpha(const bool enabled = true) override;
		virtual void clear(const Color& color) override;
		virtual void draw(const unsigned int vertices = 3) override;
		virtual void drawIndexed(const unsigned int numIndexes) override;
		virtual void setViewport(const int width, const int height) override;

	private:

		virtual const std::map<std::string, std::string>& getDefaultShaderSources() const override;
	};
}