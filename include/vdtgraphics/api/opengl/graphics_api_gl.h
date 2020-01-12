/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtgraphics/graphics_api.h>

namespace graphics
{
	class GraphicsAPI_GL : public GraphicsAPI
	{
	public:

		GraphicsAPI_GL();
		~GraphicsAPI_GL();

		virtual bool startup() override;
		virtual void shutdown() override;

		// graphics objects creation
		virtual Context* const createContext(const Context::Type type) override;
		virtual Shader* createShader(const Shader::Type type, const std::string& source) const override;
		virtual ShaderProgram* createShaderProgram(const std::initializer_list<Shader*>& shaders) const override;
		virtual Texture* createTexture(const Image& image, const Texture::Options& options = Texture::Options{}) const override;
		virtual Renderable* createRenderable(const Mesh& mesh) override;
	};
}