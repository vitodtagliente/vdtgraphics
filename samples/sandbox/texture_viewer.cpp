#include "texture_viewer.h"

#include <glad/glad.h>

#include <vdtmath/matrix4.h>

#include <vdtgraphics/context.h>
#include <vdtgraphics/image.h>
#include <vdtgraphics/index_buffer.h>
#include <vdtgraphics/renderable.h>
#include <vdtgraphics/shader.h>
#include <vdtgraphics/shader_library.h>
#include <vdtgraphics/shader_program.h>
#include <vdtgraphics/texture.h>
#include <vdtgraphics/vertex_buffer.h>

namespace graphics
{
	TextureViewer::TextureViewer(Context* const context)
	{
		m_program = createProgram(ShaderLibrary::names::TextureShader);

		std::vector<float> vertices;
		if (Image::flip_vertically)
		{
			vertices = {
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f
			};
		}
		else
		{
			vertices = {
				 1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
				-1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 1.0f
			};
		}

		unsigned int indices[] = {
			0, 1, 3, 1, 2, 3
		};

		m_renderable = std::make_unique<Renderable>();
		VertexBuffer& vb = *m_renderable->addVertexBuffer(Renderable::names::MainBuffer, vertices.size() * sizeof(float), BufferUsageMode::Static);
		vb.fillData(&vertices[0], vertices.size() * sizeof(float));
		VertexBufferLayout& layout = vb.layout;
		layout.push(VertexBufferElement("position", VertexBufferElement::Type::Float, 3));
		layout.push(VertexBufferElement("coords", VertexBufferElement::Type::Float, 2));
		IndexBuffer& ib = *m_renderable->addIndexBuffer(Renderable::names::MainBuffer, sizeof(indices), BufferUsageMode::Static);
		ib.fillData(indices, sizeof(indices));

		m_renderable->bind();
	}

	void TextureViewer::draw(Texture* const texture, const Color& color)
	{
		if (texture == nullptr) return;

		m_renderable->bind();
		m_program->bind();

		texture->bind();
		m_program->set("u_texture", 0);
		m_program->set("u_color", color.red, color.green, color.blue, color.alpha);

		const int primitiveType = GL_TRIANGLES;
		const int offset = 0;
		const int count = 6;
		const int indexType = GL_UNSIGNED_INT;

		glDrawElements(primitiveType, count, indexType, 0);
	}

	std::unique_ptr<ShaderProgram> TextureViewer::createProgram(const std::string& name)
	{
		std::map<Shader::Type, std::string> sources;
		ShaderLibrary library;
		auto it = library.getShaders().find(name);
		if (it != library.getShaders().end()
			&& Shader::Reader::parse(it->second, sources))
		{
			Shader vs(Shader::Type::Vertex, sources.find(Shader::Type::Vertex)->second);
			Shader fs(Shader::Type::Fragment, sources.find(Shader::Type::Fragment)->second);
			return std::make_unique<ShaderProgram>(std::initializer_list<Shader*>{ &vs, & fs });
		}
		return nullptr;
	}
}