#include <vdtgraphics/api/opengl/api_gl.h>

#include <vdtgraphics/api/opengl/opengl.h>
#include <vdtgraphics/api/opengl/buffer_gl.h>
#include <vdtgraphics/material.h>
#include <vdtgraphics/api/opengl/renderable_gl.h>
#include <vdtgraphics/api/opengl/renderer2d_gl.h>
#include <vdtgraphics/api/opengl/renderer3d_gl.h>
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
			return API::startup();
		}
		return false;
	}

	void API_GL::update()
	{
		API::update();
	}
	
	void API_GL::shutdown()
	{
		API::shutdown();
	}

	IndexBuffer* const API_GL::createIndexBuffer(const unsigned int indices)
	{
		return new IndexBufferGL(indices);
	}

	IndexBuffer* const API_GL::createIndexBuffer(const unsigned int indices, const BufferUsageMode usage)
	{
		return new IndexBufferGL(indices, usage);
	}

	Renderable* const API_GL::createRenderable(VertexBuffer* const vertexBuffer, IndexBuffer* const indexBuffer)
	{
		return new RenderableGL(vertexBuffer, indexBuffer);
	}

	Renderable* const API_GL::createRenderable(const Mesh& mesh)
	{
		return new RenderableGL(this, mesh);
	}

	Renderer2D* const API_GL::createRenderer2D()
	{
		Renderer2DGL* const renderer = new Renderer2DGL(this);
		renderer->initialize();
		return renderer;
	}

	Renderer3D* const API_GL::createRenderer3D()
	{
		Renderer3DGL* const renderer = new Renderer3DGL(this);
		renderer->initialize();
		return renderer;
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

	VertexBuffer* const API_GL::createVertexBuffer(const std::size_t vertexSize, const unsigned int vertices)
	{
		return new VertexBufferGL(vertexSize, vertices);
	}

	VertexBuffer* const API_GL::createVertexBuffer(const std::size_t vertexSize, const unsigned int vertices, const BufferUsageMode usage, const PrimitiveType primitiveType)
	{
		return new VertexBufferGL(vertexSize, vertices, usage, primitiveType);
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

	void API_GL::draw(const PrimitiveType primitiveType, const unsigned int vertices)
	{
		glDrawArrays(getPrimitiveTypeGL(primitiveType), 0, vertices);
	}

	void API_GL::drawIndexed(const PrimitiveType primitiveType, const unsigned int numIndexes)
	{
		glDrawElements(getPrimitiveTypeGL(primitiveType), numIndexes, GL_UNSIGNED_INT, nullptr);
	}

	void API_GL::setViewport(const int width, const int height)
	{
		glViewport(0, 0, width, height);
	}

	unsigned int API_GL::getTextureUnits() const
	{
		return 4;
	}
	
	GLenum API_GL::getPrimitiveTypeGL(const PrimitiveType primitiveType)
	{
		switch (primitiveType)
		{
		case PrimitiveType::Points: return GL_POINTS;
		case PrimitiveType::Lines: return GL_LINES;
		case PrimitiveType::LineStrip: return GL_LINE_STRIP;
		case PrimitiveType::TriangleFan: return GL_TRIANGLE_FAN;
		case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
		case PrimitiveType::Triangles:
		default:
			return GL_TRIANGLES;
		}
	}

	const std::map<std::string, std::string>& API_GL::getDefaultShaderSources() const
	{
		static const std::map<std::string, std::string> sources = {

		{ Material::Default::Name::Position, R"(
			#shader vertex

			#version 330 core

			layout (location = 0) in vec3 aPos;

			void main()
			{
				gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
			}

			#shader fragment

			#version 330 core

			out vec4 FragColor;

			void main()
			{
				FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
			} 	
		)" },

		{ Material::Default::Name::Color, R"(
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

		{ Material::Default::Name::Texture, R"(
			#shader vertex

			#version 330 core

			layout(location = 0) in vec4 position;
			layout(location = 1) in vec4 color;	
			layout(location = 2) in vec2 texCoord;
			layout(location = 3) in float textureIndex;

			out vec2 v_TexCoord;
			out vec4 v_Color;
			out float v_TextureIndex;

			uniform mat4 u_ModelViewProjectionMatrix;

			void main()
			{
				gl_Position = u_ModelViewProjectionMatrix * position;
				v_TexCoord = texCoord;
				v_Color = color;
				v_TextureIndex = textureIndex;
			}

			#shader fragment

			#version 330 core

			out vec4 fragColor;

			uniform vec4 u_Color;
			uniform sampler2D u_Textures[32];

			in vec2 v_TexCoord;
			in vec4 v_Color;
			in float v_TextureIndex;

			void main()
			{
				int index = int(v_TextureIndex);
				vec4 texColor = texture(u_Textures[index], v_TexCoord) * v_Color;
				fragColor = texColor;
			}
		)" },

		{ Material::Default::Name::CroppedTexture, R"(
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