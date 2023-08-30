#include <vdtgraphics/text_batch.h>

#include <glad/glad.h>

#include <vdtgraphics/context.h>
#include <vdtgraphics/font.h>
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
	TextBatch::TextBatch(Context* const)
	{
		m_program = createProgram(ShaderLibrary::names::TextBatchShader);

		float vertices[] =
		{
			 1.0f,  0.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.0f, 0.0f, 0.0f, 0.0f
		};

		unsigned int indices[] = {
			0, 1, 3, 1, 2, 3
		};

		m_renderable = std::make_unique<Renderable>();
		VertexBuffer& vb = *m_renderable->addVertexBuffer(Renderable::names::MainBuffer, sizeof(vertices), BufferUsageMode::Static);
		vb.fillData(vertices, sizeof(vertices));
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

	void TextBatch::setProjectionMatrix(const math::matrix4& m)
	{
		m_projectionMatrix = m;
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void TextBatch::setViewMatrix(const math::matrix4& m)
	{
		m_viewMatrix = m;
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void TextBatch::clear()
	{
		m_commands.clear();
	}

	void TextBatch::flush()
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

	void TextBatch::draw(Font* const font, const std::string& text, const math::vec3& position, const std::size_t size, const Color& color)
	{
		if (text.empty() || font == nullptr) return;

		RenderTextCommand* command = nullptr;

		for (int i = static_cast<int>(m_commands.size()) - 1; i >= 0; --i)
		{
			if (m_commands[i].hasCapacity(font))
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

		const float scale = static_cast<float>(size) / font->size;

		const float h_bearing_top = font->data['H'].bearing.y;

		float x = position.x;
		for (const char c : text)
		{
			const auto& it = font->data.find(c);
			if (it == font->data.end()) continue;

			const Glyph& glyph = it->second;
			const float x_pos = x + glyph.bearing.x * scale;
			float y_pos = position.y + (h_bearing_top - glyph.bearing.y) * scale;
			const float w = glyph.size.x * scale;
			const float h = glyph.size.y * scale;

			const math::mat4 transform = 
				math::mat4::scale(math::vec3(w, h, 1.f))
				* 
				math::matrix4::translate(math::vec3(x_pos, y_pos, position.z))
				;
			command->push({ transform, color, glyph.rect }, font);

			x += (glyph.advance) * scale;
		}
	}

	std::unique_ptr<ShaderProgram> TextBatch::createProgram(const std::string& name)
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

	TextBatch::RenderTextCommand::RenderTextCommand(Renderable* const renderable, ShaderProgram* const program, const size_t capacity)
		: RenderCommand()
		, m_capacity(capacity)
		, m_data()
		, m_fonts()
		, m_program(program)
		, m_renderable(renderable)
		, m_size(0)
	{
		m_data.reserve(capacity * (SpriteVertex::size + 1));
		m_fonts.reserve(max_font_units);
	}

	bool TextBatch::RenderTextCommand::hasCapacity(Font* const font) const
	{
		const auto& it = std::find(m_fonts.begin(), m_fonts.end(), font);
		return it != m_fonts.end() || m_fonts.size() < max_font_units;
	}

	void TextBatch::RenderTextCommand::clear()
	{
		m_data.clear();
		m_fonts.clear();
		m_size = 0;
	}

	bool TextBatch::RenderTextCommand::push(const SpriteVertex& vertex, Font* const font)
	{
		static const auto& findIndex = [](Font* const font, std::vector<Font*>& fonts) -> int
		{
			for (int i = 0; i < fonts.size(); ++i)
			{
				if (fonts[i] == font) return i;
			}

			fonts.push_back(font);
			return static_cast<int>(fonts.size()) - 1;
		};

		if (m_size < m_capacity && font != nullptr)
		{
			const float textureIndex = static_cast<float>(findIndex(font, m_fonts));
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

	RenderCommandResult TextBatch::RenderTextCommand::execute(const math::mat4& viewProjectionMatrix)
	{
		if (m_renderable == nullptr
			|| m_program == nullptr
			|| !m_program->isValid()
			|| m_fonts.empty()
			|| m_data.empty()) return RenderCommandResult::Invalid;

		m_renderable->bind();

		VertexBuffer& data = *m_renderable->findVertexBuffer("data");
		data.bind();
		data.fillData((void*)&m_data[0], m_data.size() * sizeof(float));

		m_program->bind();
		for (int i = 0; i < m_fonts.size(); ++i)
		{
			m_fonts[i]->texture->bind(i);
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