#include <vdtgraphics/api/opengl/api_gl.h>

#include <vdtgraphics/api/opengl/opengl.h>
#include <vdtgraphics/api/opengl/context_gl.h>
#include <vdtgraphics/api/opengl/renderable_gl.h>
#include <vdtgraphics/api/opengl/shader_gl.h>
#include <vdtgraphics/api/opengl/shader_library_gl.h>
#include <vdtgraphics/api/opengl/shader_program_gl.h>
#include <vdtgraphics/api/opengl/texture_gl.h>

namespace graphics
{
	GraphicsAPI_GL::GraphicsAPI_GL()
		: API(API::Type::OpenGL)
	{
		
	}

	GraphicsAPI_GL::~GraphicsAPI_GL()
	{
	}

	bool GraphicsAPI_GL::startup()
	{
		if (gladLoadGL())
		{
			m_shaderLibrary = new ShaderLibraryGL((API*)(this));
			m_shaderLibrary->initialize();

			return true;
		}
		return false;
	}
	
	void GraphicsAPI_GL::shutdown()
	{
	}
	
	Context* const GraphicsAPI_GL::createContext(const Context::Type type)
	{
		return new ContextGL((API*)(this), type);
	}

	Shader* GraphicsAPI_GL::createShader(const Shader::Type type, const std::string& source) const
	{
		return new ShaderGL(type, source);
	}
	
	ShaderProgram* GraphicsAPI_GL::createShaderProgram(const std::initializer_list<Shader*>& shaders) const
	{
		return new ShaderProgramGL(shaders);
	}
	
	Texture* GraphicsAPI_GL::createTexture(const Image& image, const Texture::Options& options) const
	{
		return new TextureGL(image, options);
	}
	
	Renderable* GraphicsAPI_GL::createRenderable(const Mesh& mesh)
	{
		return new RenderableGL(mesh);
	}
}