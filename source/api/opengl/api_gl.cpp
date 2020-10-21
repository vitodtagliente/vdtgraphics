#include <vdtgraphics/api/opengl/api_gl.h>

#include <vdtgraphics/api/opengl/opengl.h>
#include <vdtgraphics/api/opengl/renderable_gl.h>
#include <vdtgraphics/api/opengl/shader_gl.h>
#include <vdtgraphics/api/opengl/shader_program_gl.h>
#include <vdtgraphics/api/opengl/texture_gl.h>

namespace graphics
{
	API_GL::API_GL()
		: API(API::Type::OpenGL)
	{
		
	}

	API_GL::~API_GL()
	{
	}

	bool API_GL::startup()
	{
		if (gladLoadGL())
		{
			return true;
		}
		return false;
	}
	
	void API_GL::shutdown()
	{
	}

	Renderable* const API_GL::createRenderable(const Mesh& mesh)
	{
		return new RenderableGL(mesh);
	}

	Shader* const API_GL::createShader(const Shader::Type type, const std::string& source)
	{
		return new ShaderGL(type, source);
	}

	ShaderProgram* const API_GL::createShaderProgram(const std::initializer_list<Shader*>& shaders)
	{
		return new ShaderProgramGL(shaders);
	}

	Texture* const API_GL::createTexture(const Image& image, const Texture::Options& options)
	{
		return new TextureGL(image, options);
	}

	void API_GL::enableAlpha(const bool enabled)
	{
		if (enabled)
		{
			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	void API_GL::clear(const Color& color)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
	}

	void API_GL::draw(const unsigned int vertices)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices);
	}

	void API_GL::drawIndexed(const unsigned int numIndexes)
	{
		glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, nullptr);
	}

	void API_GL::setViewport(const int width, const int height)
	{
		glViewport(0, 0, width, height);
	}
}