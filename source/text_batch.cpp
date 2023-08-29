#include <vdtgraphics/text_batch.h>

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
	TextBatch::TextBatch(Context* const context)
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

	void TextBatch::flush()
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

	void TextBatch::draw(Font* const font, const std::string& text, const math::vec3& position, const std::size_t size, const Color& color)
	{
		if (text.empty() || font == nullptr) return;

		RenderTextCommand* command = nullptr;

		for (int i = static_cast<int>(m_commands.size()) - 1; i >= 0; --i)
		{
			command = dynamic_cast<RenderTextCommand*>(m_commands[i].get());
			if (command != nullptr && command->hasCapacity(font))
			{
				break;
			}
			command = nullptr;
		}

		if (command == nullptr || !command->hasCapacity(1))
		{
			command = new RenderTextCommand(
				m_renderable.get(),
				m_program.get(),
				m_viewProjectionMatrix,
				batch_size
			);
			m_commands.push_back(std::unique_ptr<RenderTextCommand>(command));
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
}