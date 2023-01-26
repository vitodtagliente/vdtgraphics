#include <vdtgraphics/render_commands.h>

#include <glad/glad.h>

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

	void RenderShapeCommand::push(const Vertex& vertex)
	{
		if (m_size < m_capacity)
		{
			m_data.insert(m_data.end(), {
				vertex.position.x, vertex.position.y, vertex.position.z,
				vertex.color.red, vertex.color.green, vertex.color.blue, vertex.color.alpha
				});
			++m_size;
		}
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

	// RenderTextureCommand
	RenderTextureCommand::RenderTextureCommand(Renderable* const renderable, ShaderProgram* const program, Texture* const texture, const math::mat4& viewProjectionMatrix, const size_t capacity)
		: RenderCommand()
		, m_capacity(capacity)
		, m_data()
		, m_program(program)
		, m_renderable(renderable)
		, m_size(0)
		, m_texture(texture)
		, m_viewProjectionMatrix(viewProjectionMatrix)
	{
		m_data.reserve(capacity * SpriteVertex::size);
	}

	void RenderTextureCommand::push(const SpriteVertex& vertex)
	{
		if (m_size < m_capacity)
		{
			m_data.insert(m_data.end(), {
				vertex.rect.x, vertex.rect.y, vertex.rect.width, vertex.rect.height,
				vertex.color.red, vertex.color.green, vertex.color.blue, vertex.color.alpha
				});
			m_data.insert(m_data.end(), vertex.transform.data, vertex.transform.data + vertex.transform.length);
			++m_size;
		}
	}

	RenderCommandResult RenderTextureCommand::execute()
	{
		if (m_renderable == nullptr
			|| m_program == nullptr
			|| !m_program->isValid()
			|| m_texture == nullptr
			|| !m_texture->isValid()
			|| m_data.empty()) return RenderCommandResult::Invalid;

		m_renderable->bind();

		VertexBuffer& data = *m_renderable->findVertexBuffer("data");
		data.bind();
		data.fillData((void*)&m_data[0], m_data.size() * sizeof(float));

		m_program->bind();
		m_texture->bind(0);
		m_program->set("u_texture", 0);
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