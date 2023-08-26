#include <vdtgraphics/sprite_batch.h>

#include <vdtgraphics/context.h>
#include <vdtgraphics/image.h>
#include <vdtgraphics/index_buffer.h>
#include <vdtgraphics/renderable.h>
#include <vdtgraphics/render_commands.h>
#include <vdtgraphics/shader.h>
#include <vdtgraphics/shader_library.h>
#include <vdtgraphics/shader_program.h>
#include <vdtgraphics/texture.h>
#include <vdtgraphics/vertex_buffer.h>

namespace graphics
{
	SpriteBatch::SpriteBatch(Context* const context)
	{
		m_program = createProgram(ShaderLibrary::names::SpriteBatchShader);

		std::vector<float> vertices;
		if (Image::flip_vertically)
		{
			vertices = {
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
			};
		}
		else
		{
			vertices = {
				 0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
				-0.5f, -0.5f, 0.0f, 0.0f, 1.0f
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

		VertexBuffer& dataBuffer = *m_renderable->addVertexBuffer("data", SpriteVertex::size * 10000 * sizeof(float), BufferUsageMode::Stream);
		dataBuffer.layout.push(VertexBufferElement("texture", VertexBufferElement::Type::Float, 1, true, true));
		dataBuffer.layout.push(VertexBufferElement("crop", VertexBufferElement::Type::Float, 4, true, true));
		dataBuffer.layout.push(VertexBufferElement("color", VertexBufferElement::Type::Float, 4, true, true));
		dataBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
		dataBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
		dataBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
		dataBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
		dataBuffer.layout.startingIndex = 2;

		m_renderable->bind();
	}

	void SpriteBatch::setProjectionMatrix(const math::matrix4& m)
	{
		m_projectionMatrix = m;
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void SpriteBatch::setViewMatrix(const math::matrix4& m)
	{
		m_viewMatrix = m;
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void SpriteBatch::flush()
	{
		m_stats.drawCalls = 0;

		for (const auto& command : m_commands)
		{
			if (command->execute() == RenderCommandResult::OK)
			{
				++m_stats.drawCalls;
			}
		}
		m_commands.clear();
	}

	void SpriteBatch::draw(Texture* const texture, const math::mat4& transform, const TextureRect& rect, const Color& color)
	{
		if (texture == nullptr) return;

		RenderTextureCommand* command = nullptr;

		for (int i = static_cast<int>(m_commands.size()) - 1; i >= 0; --i)
		{
			command = dynamic_cast<RenderTextureCommand*>(m_commands[i].get());
			if (command != nullptr && command->hasCapacity(texture))
			{
				break;
			}
			command = nullptr;
		}

		if (command == nullptr || !command->hasCapacity(1))
		{
			command = new RenderTextureCommand(
				m_renderable.get(),
				m_program.get(),
				m_viewProjectionMatrix,
				batch_size
			);
			m_commands.push_back(std::unique_ptr<RenderTextureCommand>(command));
		}

		command->push({ transform, color, rect }, texture);
	}

	void SpriteBatch::draw(Texture* const texture, const math::vec3& position, const TextureRect& rect, const Color& color)
	{
		draw(texture, math::matrix4::translate(position), rect, color);
	}

	void SpriteBatch::draw(Texture* const texture, const math::vec3& position, const float rotation, const TextureRect& rect, const Color& color)
	{
		draw(texture, math::matrix4::translate(position) * math::matrix4::rotate_z(rotation), rect, color);
	}

	void SpriteBatch::draw(Texture* const texture, const math::vec3& position, const math::vec3& scale, const TextureRect& rect, const Color& color)
	{
		draw(texture, math::matrix4::scale(scale) * math::matrix4::translate(position), rect, color);
	}

	void SpriteBatch::draw(Texture* const texture, const math::vec3& position, const float rotation, const math::vec3& scale, const TextureRect& rect, const Color& color)
	{
		draw(texture, math::matrix4::scale(scale) * math::matrix4::rotate_z(rotation) * math::matrix4::translate(position), rect, color);
	}

	std::unique_ptr<ShaderProgram> SpriteBatch::createProgram(const std::string& name)
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