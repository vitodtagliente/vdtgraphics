#include <vdtgraphics/api/opengl/renderer_gl.h>
#include <vdtgraphics/api/opengl/opengl.h>
#include <vdtgraphics/api/opengl/graphics_api_gl.h>
#include <vdtgraphics/shaders.h>

namespace graphics
{
	RendererGL::RendererGL(GraphicsAPI* const api)
		: Renderer(api)
	{
	}

	void RendererGL::enableAlpha(const bool bEnabled)
	{
		if (bEnabled)
		{
			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	void RendererGL::clear(const Color& color)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
	}

	void RendererGL::draw(const unsigned int vertices)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices);
	}

	void RendererGL::drawIndexed(const unsigned int vertices)
	{
		glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, nullptr);
	}
	
	void RendererGL::setViewport(const int width, const int height)
	{
		glViewport(0, 0, width, height);
	}

	void RendererGL::setDrawingMode(const DrawingMode drawingMode)
	{
		switch (drawingMode)
		{
		case Renderer::DrawingMode::Lines:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case Renderer::DrawingMode::Fill:
		default:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}
	}
	
	const std::map<std::string, std::string>& RendererGL::getDefaultShaderSources() const
	{
		static const std::map<std::string, std::string> sources = {

		{ Shaders::names::ColorShader, R"(
			#shader vertex

			#version 330 core

			layout(location = 0) in vec4 in_vertex;

			uniform mat4 u_ModelViewProjectionMatrix;

			void main()
			{
				gl_Position = u_ModelViewProjectionMatrix * in_vertex;
			}

			#shader fragment

			#version 330 core

			out vec4 fragColor;

			uniform vec4 u_Color;

			void main()
			{
				fragColor = u_Color;
			}			
		)" },

		{ Shaders::names::TextureShader, R"(
			#shader vertex

			#version 330 core

			layout(location = 0) in vec4 position;
			layout(location = 1) in vec2 texCoord;

			out vec2 v_TexCoord;

			uniform mat4 u_ModelViewProjectionMatrix;

			void main()
			{
				gl_Position = u_ModelViewProjectionMatrix * position;
				v_TexCoord = texCoord;
			}

			#shader fragment

			#version 330 core

			out vec4 fragColor;

			uniform vec4 u_Color;
			uniform sampler2D u_Texture;

			in vec2 v_TexCoord;

			void main()
			{
				vec4 texColor = texture(u_Texture, v_TexCoord);
				fragColor = texColor;
			}
		)" },

		{ Shaders::names::CroppedTextureShader, R"(
			#shader vertex

			#version 330 core

			layout(location = 0) in vec4 position;
			layout(location = 1) in vec2 texCoord;

			out vec2 v_TexCoord;

			uniform mat4 u_ModelViewProjectionMatrix;

			void main()
			{
				gl_Position = u_ModelViewProjectionMatrix * position;
				v_TexCoord = texCoord;
			}

			#shader fragment

			#version 330 core

			out vec4 fragColor;

			uniform vec4 u_Color;
			uniform sampler2D u_Texture;
			uniform vec4 u_TextureCropping;

			in vec2 v_TexCoord;

			void main()
			{
				vec2 croppingCoords;
				croppingCoords.x = (v_TexCoord.x * u_TextureCropping.z + u_TextureCropping.x);
				croppingCoords.y = (v_TexCoord.y * u_TextureCropping.w + u_TextureCropping.y);
				vec4 texColor = texture(u_Texture, croppingCoords);
				fragColor = texColor;
			}
		)" }

		};

		return sources;
	}
}