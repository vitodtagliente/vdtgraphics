#include <vdtgraphics/sprite_batch.h>

#include <glad/glad.h>

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

	void SpriteBatch::clear()
	{
		m_last_used_commands = 0;
		m_commands.clear();
	}

	void SpriteBatch::flush()
	{
		m_stats.draw_calls = 0;
		m_stats.draw_entities = 0;

		for (auto& command : m_commands)
		{
			if (command.execute(m_viewProjectionMatrix) == RenderCommandResult::OK)
			{
				m_stats.draw_entities += command.size();
				++m_stats.draw_calls;
			}
			command.clear();
		}

		const std::size_t commands_size = m_commands.size();
		while (m_last_used_commands > m_commands.size())
		{
			m_commands.pop_back();
		}
		m_last_used_commands = commands_size;
	}

	void SpriteBatch::draw(Texture* const texture, const math::mat4& transform, const TextureRect& rect, const Color& color)
	{
		if (texture == nullptr) return;

		RenderSpriteCommand* command = nullptr;

		for (int i = static_cast<int>(m_commands.size()) - 1; i >= 0; --i)
		{
			if (m_commands[i].hasCapacity(texture))
			{
				command = &m_commands[i];
				break;
			}
		}

		if (command == nullptr)
		{
			command = &m_commands.emplace_back(
				m_renderable.get(),
				m_program.get(),
				batch_size
			);
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

	SpriteBatch::RenderSpriteCommand::RenderSpriteCommand(Renderable* const renderable, ShaderProgram* const program, const size_t capacity)
		: RenderCommand()
		, m_capacity(capacity)
		, m_data()
		, m_program(program)
		, m_renderable(renderable)
		, m_size(0)
		, m_textures()
	{
		m_data.reserve(capacity * (SpriteVertex::size + 1));
		m_textures.reserve(max_texture_units);
	}

	bool SpriteBatch::RenderSpriteCommand::hasCapacity(Texture* const texture) const
	{
		const auto& it = std::find(m_textures.begin(), m_textures.end(), texture);
		return it != m_textures.end() || m_textures.size() < max_texture_units;
	}

	bool SpriteBatch::RenderSpriteCommand::push(const SpriteVertex& vertex, Texture* const texture)
	{
		static const auto& findIndex = [](Texture* const texture, std::vector<Texture*>& textures) -> int
		{
			for (int i = 0; i < textures.size(); ++i)
			{
				if (textures[i] == texture) return i;
			}

			textures.push_back(texture);
			return static_cast<int>(textures.size()) - 1;
		};

		if (m_size < m_capacity && texture != nullptr)
		{
			const float textureIndex = static_cast<float>(findIndex(texture, m_textures));
			m_data.insert(m_data.end(), {
				textureIndex,
				vertex.rect.x, vertex.rect.y, vertex.rect.width, vertex.rect.height,
				vertex.color.red, vertex.color.green, vertex.color.blue, vertex.color.alpha
				});
			m_data.insert(m_data.end(), vertex.transform.data, vertex.transform.data + vertex.transform.length);
			++m_size;
			return true;
		}
		return false;
	}

	void SpriteBatch::RenderSpriteCommand::clear()
	{
		m_data.clear();
		m_textures.clear();
		m_size = 0;
	}

	RenderCommandResult SpriteBatch::RenderSpriteCommand::execute(const math::mat4& viewProjectionMatrix)
	{
		if (m_renderable == nullptr
			|| m_program == nullptr
			|| !m_program->isValid()
			|| m_textures.empty()
			|| m_data.empty()) return RenderCommandResult::Invalid;

		m_renderable->bind();

		VertexBuffer& data = *m_renderable->findVertexBuffer("data");
		data.bind();
		data.fillData((void*)&m_data[0], m_data.size() * sizeof(float));

		m_program->bind();
		for (int i = 0; i < m_textures.size(); ++i)
		{
			m_textures[i]->bind(i);
			m_program->set("u_texture" + std::to_string(i), i);
		}
		m_program->set("u_matrix", viewProjectionMatrix);

		const int primitiveType = GL_TRIANGLES;
		const int offset = 0;
		const int count = 6;
		const int numInstances = static_cast<int>(m_size);
		const int indexType = GL_UNSIGNED_INT;

		glDrawElementsInstanced(primitiveType, count, indexType, offset, numInstances);
		return RenderCommandResult::OK;
	}
}