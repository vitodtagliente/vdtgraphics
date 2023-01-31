#include <vdtgraphics/render_commands.h>

#include <glad/glad.h>

#include <vdtgraphics/font.h>
#include <vdtgraphics/renderable.h>
#include <vdtgraphics/shader_program.h>
#include <vdtgraphics/texture.h>
#include <vdtgraphics/vertex_buffer.h>

namespace graphics
{
	// RenderShapeCommand
	RenderShapeCommand::RenderShapeCommand(Renderable* const renderable, ShaderProgram* const program, const math::mat4& viewProjectionMatrix, const ShapeRenderStyle style, const size_t capacity)
		: RenderCommand()
		, m_capacity(capacity)
		, m_data()
		, m_program(program)
		, m_renderable(renderable)
		, m_size(0)
		, m_style(style)
		, m_viewProjectionMatrix(viewProjectionMatrix)
	{
		m_data.reserve(capacity * Vertex::size);
	}

	bool RenderShapeCommand::push(const Vertex& vertex)
	{
		if (m_size < m_capacity)
		{
			m_data.insert(m_data.end(), {
				vertex.position.x, vertex.position.y, vertex.position.z,
				vertex.color.red, vertex.color.green, vertex.color.blue, vertex.color.alpha
				});
			++m_size;
			return true;
		}
		return false;
	}

	RenderCommandResult RenderShapeCommand::execute()
	{
		if (m_renderable == nullptr
			|| m_program == nullptr
			|| !m_program->isValid()
			|| m_data.empty()) return RenderCommandResult::Invalid;

		m_renderable->bind();

		VertexBuffer* vertexBuffer = m_renderable->findVertexBuffer(Renderable::names::MainBuffer);
		vertexBuffer->bind();
		vertexBuffer->fillData((void*)&m_data[0], m_data.size() * sizeof(float));

		m_program->bind();
		m_program->set("u_matrix", m_viewProjectionMatrix);

		const int primitiveType = m_style == ShapeRenderStyle::fill ? GL_TRIANGLES : GL_LINES;
		const int offset = 0;
		const int count = static_cast<int>(m_data.size()) / static_cast<int>(Vertex::size);

		glDrawArrays(primitiveType, offset, count);
		return RenderCommandResult::OK;
	}

	// RenderTextCommand
	RenderTextCommand::RenderTextCommand(Renderable* const renderable, ShaderProgram* const program, const math::mat4& viewProjectionMatrix, const size_t capacity)
		: RenderCommand()
		, m_capacity(capacity)
		, m_data()
		, m_fonts()
		, m_program(program)
		, m_renderable(renderable)
		, m_size(0)
		, m_viewProjectionMatrix(viewProjectionMatrix)
	{
		m_data.reserve(capacity * (SpriteVertex::size + 1));
		m_fonts.reserve(max_font_units);
	}

	bool RenderTextCommand::hasCapacity(Font* const font) const
	{
		const auto& it = std::find(m_fonts.begin(), m_fonts.end(), font);
		return it != m_fonts.end() || m_fonts.size() < max_font_units;
	}

	bool RenderTextCommand::push(const SpriteVertex& vertex, Font* const font)
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

	RenderCommandResult RenderTextCommand::execute()
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
		m_program->set("u_matrix", m_viewProjectionMatrix);

		const int primitiveType = GL_TRIANGLES;
		const int offset = 0;
		const int count = 6;
		const int numInstances = static_cast<int>(m_size);
		const int indexType = GL_UNSIGNED_INT;

		glDrawElementsInstanced(primitiveType, count, indexType, offset, numInstances);
		return RenderCommandResult::OK;
	}

	// RenderTextureCommand
	RenderTextureCommand::RenderTextureCommand(Renderable* const renderable, ShaderProgram* const program, const math::mat4& viewProjectionMatrix, const size_t capacity)
		: RenderCommand()
		, m_capacity(capacity)
		, m_data()
		, m_program(program)
		, m_renderable(renderable)
		, m_size(0)
		, m_textures()
		, m_viewProjectionMatrix(viewProjectionMatrix)
	{
		m_data.reserve(capacity * (SpriteVertex::size + 1));
		m_textures.reserve(max_texture_units);
	}

	bool RenderTextureCommand::hasCapacity(Texture* const texture) const
	{
		const auto& it = std::find(m_textures.begin(), m_textures.end(), texture);
		return it != m_textures.end() || m_textures.size() < max_texture_units;
	}

	bool RenderTextureCommand::push(const SpriteVertex& vertex, Texture* const texture)
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

	RenderCommandResult RenderTextureCommand::execute()
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
		m_program->set("u_matrix", m_viewProjectionMatrix);

		const int primitiveType = GL_TRIANGLES;
		const int offset = 0;
		const int count = 6;
		const int numInstances = static_cast<int>(m_size);
		const int indexType = GL_UNSIGNED_INT;

		glDrawElementsInstanced(primitiveType, count, indexType, offset, numInstances);
		return RenderCommandResult::OK;
	}
}