#include <vdtgraphics/opengl/graphics_api_gl.h>
#include <vdtgraphics/opengl/opengl.h>
#include <vdtgraphics/opengl/renderable_gl.h>
#include <vdtgraphics/opengl/shader_gl.h>
#include <vdtgraphics/opengl/shader_program_gl.h>
#include <vdtgraphics/opengl/texture_gl.h>

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
	
	void GraphicsAPI_GL::clear(const Color& color)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(color.red(), color.green(), color.blue(), color.alpha());
	}
	
	void GraphicsAPI_GL::setViewport(const unsigned int width, const unsigned int height)
	{
		glViewport(0, 0, width, height);
	}
	
	void GraphicsAPI_GL::draw(const unsigned int vertices)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices);
	}
	
	void GraphicsAPI_GL::drawIndexed(const unsigned int indices)
	{
		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);
	}
	
	void GraphicsAPI_GL::enableAlpha(const bool enabled)
	{
		if (enabled)
		{
			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
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
		return new TextureGL(data, width, height, components, options);
	}
	
	Renderable* GraphicsAPI_GL::createRenderable(const Mesh& mesh)
	{
		return new RenderableGL(mesh);
	}
}