#include <vdtgraphics/primitive_batch.h>

#include <glad/glad.h>

#include <vdtgraphics/context.h>
#include <vdtgraphics/index_buffer.h>
#include <vdtgraphics/renderable.h>
#include <vdtgraphics/shader.h>
#include <vdtgraphics/shader_library.h>
#include <vdtgraphics/shader_program.h>
#include <vdtgraphics/vertex_buffer.h>

namespace graphics
{
	PrimitiveBatch::PrimitiveBatch(Context* const)
	{
		m_program = createProgram(ShaderLibrary::names::PrimitiveBatchShader);
		
		// fill
		{
			m_fill_renderable = std::make_unique<Renderable>();
			VertexBuffer& vb = *m_fill_renderable->addVertexBuffer(Renderable::names::MainBuffer, Vertex::size * 10000 * sizeof(float), BufferUsageMode::Static);
			VertexBufferLayout& layout = vb.layout;
			layout.push(VertexBufferElement("position", VertexBufferElement::Type::Float, 3));
			layout.push(VertexBufferElement("color", VertexBufferElement::Type::Float, 4));
		}

		// stroke
		{
			m_stroke_renderable = std::make_unique<Renderable>();
			VertexBuffer& vb = *m_stroke_renderable->addVertexBuffer(Renderable::names::MainBuffer, Vertex::size * 10000 * sizeof(float), BufferUsageMode::Static);
			VertexBufferLayout& layout = vb.layout;
			layout.push(VertexBufferElement("position", VertexBufferElement::Type::Float, 3));
			layout.push(VertexBufferElement("color", VertexBufferElement::Type::Float, 4));
		}
	}

	void PrimitiveBatch::setProjectionMatrix(const math::matrix4& m)
	{
		m_projectionMatrix = m;
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void PrimitiveBatch::setViewMatrix(const math::matrix4& m)
	{
		m_viewMatrix = m;
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void PrimitiveBatch::flush()
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

	void PrimitiveBatch::drawCircle(ShapeRenderStyle style, const math::vec3& position, float radius, const Color& color)
	{
		static const unsigned int s_triangles = 20; // number of triangles
		static const float s_twoPi = 2.0f * math::pi;
		static const float s_delta = s_twoPi / s_triangles;

		std::vector<Vertex> vertices;
		vertices.reserve((style == ShapeRenderStyle::fill ? 3 : 2) * s_triangles);

		for (int i = 0; i < s_triangles; ++i)
		{
			const float angle = i * s_delta;
			const float nextAngle = (i + 1) * s_delta;

			if (style == ShapeRenderStyle::fill)
			{
				vertices.push_back({ position, color });
				vertices.push_back({ position + math::vec3(radius * std::sin(angle), radius * std::cos(angle), 0.f), color });
				vertices.push_back({ position + math::vec3(radius * std::sin(nextAngle), radius * std::cos(nextAngle), 0.f), color });
			}
			else
			{
				vertices.push_back({ position + math::vec3(radius * std::sin(angle), radius * std::cos(angle), 0.f), color });
				vertices.push_back({ position + math::vec3(radius * std::sin(nextAngle), radius * std::cos(nextAngle), 0.f), color });
			}
		}
		drawShape(style, vertices);
	}

	void PrimitiveBatch::drawLine(const math::vec3& point1, const Color& color1, const math::vec3& point2, const Color& color2)
	{
		std::vector<Vertex> vertices = {
			   { point1, color1 },
			   { point2, color2 }
		};
		drawShape(ShapeRenderStyle::stroke, vertices);
	}

	void PrimitiveBatch::drawShape(ShapeRenderStyle style, const std::vector<Vertex>& vertices)
	{
		RenderPrimitiveCommand* command = nullptr;

		for (int i = static_cast<int>(m_commands.size()) - 1; i >= 0; --i)
		{
			if (m_commands[i].getStyle() == style)
			{
				command = &m_commands[i];
				break;
			}
		}

		if (command == nullptr)
		{
			command = &m_commands.emplace_back(
				style == ShapeRenderStyle::fill ? m_fill_renderable.get() : m_stroke_renderable.get(),
				m_program.get(),
				style,
				batch_size
			);
		}

		for (const auto& vertex : vertices)
		{
			command->push(vertex);
		}
	}

	void PrimitiveBatch::drawRect(ShapeRenderStyle style, const math::vec3& position, float width, float height, const Color& color)
	{
		const float w = width / 2;
		const float h = height / 2;

		if (style == ShapeRenderStyle::fill)
		{
			std::vector<Vertex> vertices = {
				{ position + math::vec3(w, h, 0.f), color },
				{ position + math::vec3(-w, h, 0.f), color },
				{ position + math::vec3(-w, -h, 0.f), color },
				{ position + math::vec3(-w, -h, 0.f), color },
				{ position + math::vec3(w, -h, 0.f), color },
				{ position + math::vec3(w, h, 0.f), color }
			};
			drawShape(style, vertices);
		}
		else
		{
			std::vector<Vertex> vertices = {
				{ position + math::vec3(w, h, 0.f), color },
				{ position + math::vec3(-w, h, 0.f), color },
				{ position + math::vec3(-w, h, 0.f), color },
				{ position + math::vec3(-w, -h, 0.f), color },
				{ position + math::vec3(-w, -h, 0.f), color },
				{ position + math::vec3(w, -h, 0.f), color },
				{ position + math::vec3(w, -h, 0.f), color },
				{ position + math::vec3(w, h, 0.f), color }
			};
			drawShape(style, vertices);
		}
	}

	std::unique_ptr<ShaderProgram> PrimitiveBatch::createProgram(const std::string& name)
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

	PrimitiveBatch::RenderPrimitiveCommand::RenderPrimitiveCommand(Renderable* const renderable, ShaderProgram* const program, const ShapeRenderStyle style, const size_t capacity)
		: RenderCommand()
		, m_capacity(capacity)
		, m_data()
		, m_program(program)
		, m_renderable(renderable)
		, m_size(0)
		, m_style(style)
	{
		m_data.reserve(capacity * Vertex::size);
	}

	bool PrimitiveBatch::RenderPrimitiveCommand::push(const Vertex& vertex)
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

	void PrimitiveBatch::RenderPrimitiveCommand::clear()
	{
		m_data.clear();
		m_size = 0;
	}

	RenderCommandResult PrimitiveBatch::RenderPrimitiveCommand::execute(const math::mat4& viewProjectionMatrix)
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
		m_program->set("u_matrix", viewProjectionMatrix);

		const int primitiveType = m_style == ShapeRenderStyle::fill ? GL_TRIANGLES : GL_LINES;
		const int offset = 0;
		const int count = static_cast<int>(m_data.size()) / static_cast<int>(Vertex::size);

		glDrawArrays(primitiveType, offset, count);
		return RenderCommandResult::OK;
	}
}