#include <vdtgraphics/api/opengl/graphics_api_gl.h>
#include <vdtgraphics/api/opengl/opengl.h>
#include <vdtgraphics/api/opengl/renderable_gl.h>
#include <vdtgraphics/api/opengl/renderer_gl.h>
#include <vdtgraphics/api/opengl/shader_gl.h>
#include <vdtgraphics/api/opengl/shader_program_gl.h>
#include <vdtgraphics/api/opengl/texture_gl.h>

namespace graphics
{
	GraphicsAPI_GL::GraphicsAPI_GL()
		: GraphicsAPI(GraphicsAPI::Type::OpenGL)
	{
	}

	GraphicsAPI_GL::~GraphicsAPI_GL()
	{
	}

	bool GraphicsAPI_GL::startup()
	{
		return gladLoadGL();
	}
	
	void GraphicsAPI_GL::shutdown()
	{
	}
	
	Shader* GraphicsAPI_GL::createShader(const Shader::Type type, const std::string& source) const
	{
		return new ShaderGL(type, source);
	}
	
	ShaderProgram* GraphicsAPI_GL::createShaderProgram(const std::initializer_list<Shader*>& shaders) const
	{
		return new ShaderProgramGL(shaders);
	}
	
	Texture* GraphicsAPI_GL::createTexture(const unsigned char* const data, const unsigned int width, const unsigned int height, const unsigned int components, const Texture::Options& options) const
	{
		// IMAGE REFACTORING
		// return new TextureGL(data, width, height, components, options);
		return nullptr;
	}
	
	Renderable* GraphicsAPI_GL::createRenderable(const Mesh& mesh)
	{
		return new RenderableGL(mesh);
	}
	
	Renderer* const GraphicsAPI_GL::createRenderer() const
	{
		return new RendererGL((GraphicsAPI*)(this));
	}
}