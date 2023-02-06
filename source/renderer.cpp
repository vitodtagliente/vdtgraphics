#include <vdtgraphics/renderer.h>

#include <glad/glad.h>

#include <vdtgraphics/context.h>
#include <vdtgraphics/font.h>
#include <vdtgraphics/image.h>
#include <vdtgraphics/index_buffer.h>
#include <vdtgraphics/renderable.h>
#include <vdtgraphics/render_command.h>
#include <vdtgraphics/render_commands.h>
#include <vdtgraphics/render_target.h>
#include <vdtgraphics/shader.h>
#include <vdtgraphics/shader_library.h>
#include <vdtgraphics/shader_program.h>
#include <vdtgraphics/texture.h>
#include <vdtgraphics/vertex_buffer.h>

namespace graphics
{
	bool Renderer::init(Context* const context)
	{
		if (context == nullptr || context->getState() != Context::State::Initialized)
		{
			return false;
		}

		m_shaderLibrary = std::make_unique<ShaderLibrary>();

		// color
		{
			m_colorProgram = createProgram(ShaderLibrary::names::ColorShader);
		}
		// shapes
		m_shapeProgram = createProgram(ShaderLibrary::names::PolygonBatchShader);
		// fill
		{
			m_shapeFillRenderable = std::make_unique<Renderable>();
			VertexBuffer& vb = *m_shapeFillRenderable->addVertexBuffer(Renderable::names::MainBuffer, Vertex::size * 10000 * sizeof(float), BufferUsageMode::Static);
			VertexBufferLayout& layout = vb.layout;
			layout.push(VertexBufferElement("position", VertexBufferElement::Type::Float, 3));
			layout.push(VertexBufferElement("color", VertexBufferElement::Type::Float, 4));
		}
		// stroke
		{
			m_shapeStrokeRenderable = std::make_unique<Renderable>();
			VertexBuffer& vb = *m_shapeStrokeRenderable->addVertexBuffer(Renderable::names::MainBuffer, Vertex::size * 10000 * sizeof(float), BufferUsageMode::Static);
			VertexBufferLayout& layout = vb.layout;
			layout.push(VertexBufferElement("position", VertexBufferElement::Type::Float, 3));
			layout.push(VertexBufferElement("color", VertexBufferElement::Type::Float, 4));
		}
		// text
		{
			m_textProgram = createProgram(ShaderLibrary::names::TextShader);

			float vertices[] =
			{
				 0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
				-0.5f, -0.5f, 0.0f, 0.0f, 1.0f
			};

			unsigned int indices[] = {
				0, 1, 3, 1, 2, 3
			};

			m_textRenderable = std::make_unique<Renderable>();
			VertexBuffer& vb = *m_textRenderable->addVertexBuffer(Renderable::names::MainBuffer, sizeof(vertices), BufferUsageMode::Static);
			vb.fillData(vertices, sizeof(vertices));
			VertexBufferLayout& layout = vb.layout;
			layout.push(VertexBufferElement("position", VertexBufferElement::Type::Float, 3));
			layout.push(VertexBufferElement("coords", VertexBufferElement::Type::Float, 2));
			IndexBuffer& ib = *m_textRenderable->addIndexBuffer(Renderable::names::MainBuffer, sizeof(indices), BufferUsageMode::Static);
			ib.fillData(indices, sizeof(indices));

			VertexBuffer& dataBuffer = *m_textRenderable->addVertexBuffer("data", SpriteVertex::size * 10000 * sizeof(float), BufferUsageMode::Stream);
			dataBuffer.layout.push(VertexBufferElement("texture", VertexBufferElement::Type::Float, 1, true, true));
			dataBuffer.layout.push(VertexBufferElement("crop", VertexBufferElement::Type::Float, 4, true, true));
			dataBuffer.layout.push(VertexBufferElement("color", VertexBufferElement::Type::Float, 4, true, true));
			dataBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			dataBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			dataBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			dataBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			dataBuffer.layout.startingIndex = 2;

			m_textRenderable->bind();
		}
		// textures
		{
			m_spriteProgram = createProgram(ShaderLibrary::names::SpriteBatchShader);

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

			m_textureRenderable = std::make_unique<Renderable>();
			VertexBuffer& vb = *m_textureRenderable->addVertexBuffer(Renderable::names::MainBuffer, vertices.size() * sizeof(float), BufferUsageMode::Static);
			vb.fillData(&vertices[0], vertices.size() * sizeof(float));
			VertexBufferLayout& layout = vb.layout;
			layout.push(VertexBufferElement("position", VertexBufferElement::Type::Float, 3));
			layout.push(VertexBufferElement("coords", VertexBufferElement::Type::Float, 2));
			IndexBuffer& ib = *m_textureRenderable->addIndexBuffer(Renderable::names::MainBuffer, sizeof(indices), BufferUsageMode::Static);
			ib.fillData(indices, sizeof(indices));

			VertexBuffer& dataBuffer = *m_textureRenderable->addVertexBuffer("data", SpriteVertex::size * 10000 * sizeof(float), BufferUsageMode::Stream);
			dataBuffer.layout.push(VertexBufferElement("texture", VertexBufferElement::Type::Float, 1, true, true));
			dataBuffer.layout.push(VertexBufferElement("crop", VertexBufferElement::Type::Float, 4, true, true));
			dataBuffer.layout.push(VertexBufferElement("color", VertexBufferElement::Type::Float, 4, true, true));
			dataBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			dataBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			dataBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			dataBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			dataBuffer.layout.startingIndex = 2;

			m_textureRenderable->bind();
		}
		// texture
		{
			m_textureProgram = createProgram(ShaderLibrary::names::TextureShader);
		}
		return true;
	}

	void Renderer::uninit()
	{
	}

	void Renderer::clear(const Color& color)
	{
		stats.drawCalls = 0;
		m_commands.clear();
		glClearColor(color.red, color.green, color.blue, color.alpha);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::setViewport(const int width, const int height)
	{
		glViewport(0, 0, width, height);
	}

	void Renderer::setWireframeMode(const bool enabled)
	{
		if (enabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void Renderer::setRenderTarget(RenderTarget* const renderTarget)
	{
		if (renderTarget == nullptr || !renderTarget->isValid())
		{
			// flush before switching target
			flush();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return;
		}

		if (renderTarget != renderTarget)
		{
			flush();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->id());
		setViewport(renderTarget->getWidth(), renderTarget->getHeight());
		clear(renderTarget->getColor());
		m_renderTarget = renderTarget;
	}

	void Renderer::setProjectionMatrix(const math::matrix4& m)
	{
		m_projectionMatrix = m;
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void Renderer::setViewMatrix(const math::matrix4& m)
	{
		m_viewMatrix = m;
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void Renderer::submit(std::unique_ptr<RenderCommand> command)
	{
		m_commands.push_back(std::move(command));
	}

	void Renderer::flush()
	{
		for (const auto& command : m_commands)
		{
			if (command->execute() == RenderCommandResult::OK)
			{
				++stats.drawCalls;
			}
		}
		m_commands.clear();
	}

	std::unique_ptr<ShaderProgram> Renderer::createProgram(const std::string& name)
	{
		std::map<Shader::Type, std::string> sources;
		auto it = m_shaderLibrary->getShaders().find(name);
		if (it != m_shaderLibrary->getShaders().end()
			&& Shader::Reader::parse(it->second, sources))
		{
			Shader vs(Shader::Type::Vertex, sources.find(Shader::Type::Vertex)->second);
			Shader fs(Shader::Type::Fragment, sources.find(Shader::Type::Fragment)->second);
			return std::make_unique<ShaderProgram>(std::initializer_list<Shader*>{ &vs, & fs });
		}
		return nullptr;
	}

	void Renderer::submitDrawCircle(const ShapeRenderStyle style, const math::vec3& position, float radius, const Color& color)
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
		submitDrawShape(style, vertices);
	}

	void Renderer::submitDrawLine(const math::vec3& point1, const Color& color1, const math::vec3& point2, const Color& color2)
	{
		std::vector<Vertex> vertices = {
			{ point1, color1 },
			{ point2, color2 }
		};
		submitDrawShape(ShapeRenderStyle::stroke, vertices);
	}

	void Renderer::submitDrawShape(const ShapeRenderStyle style, const std::vector<Vertex>& vertices)
	{
		RenderShapeCommand* command = nullptr;

		for (int i = static_cast<int>(m_commands.size()) - 1; i >= 0; --i)
		{
			command = dynamic_cast<RenderShapeCommand*>(m_commands[i].get());
			if (command != nullptr && command->getStyle() == style)
			{
				break;
			}
			command = nullptr;
		}

		if (command == nullptr || !command->hasCapacity(vertices.size()))
		{
			command = new RenderShapeCommand(
				style == ShapeRenderStyle::fill ? m_shapeFillRenderable.get() : m_shapeStrokeRenderable.get(),
				m_shapeProgram.get(),
				m_viewProjectionMatrix,
				style,
				1000
			);
			m_commands.push_back(std::unique_ptr<RenderShapeCommand>(command));
		}

		for (const auto& vertex : vertices)
		{
			command->push(vertex);
		}
	}

	void Renderer::submitDrawRect(const ShapeRenderStyle style, const math::vec3& position, float width, float height, const Color& color)
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
			submitDrawShape(style, vertices);
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
			submitDrawShape(style, vertices);
		}
	}

	void Renderer::submitDrawText(Font* const font, const std::string& text, const math::vec3& position, const float scale, const Color& color)
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
				m_textRenderable.get(),
				m_textProgram.get(),
				m_viewProjectionMatrix,
				10000
			);
			m_commands.push_back(std::unique_ptr<RenderTextCommand>(command));
		}

		float x = position.x;
		for (const char c : text)
		{
			const auto& it = font->data.find(c);
			if (it == font->data.end()) continue;

			const Glyph& glyph = it->second;
			const float x_pos = x + glyph.bearing.x * scale;
			const float y_pos = position.y + (glyph.size.y - glyph.bearing.y) * scale;
			command->push({ math::matrix4::scale(math::vec3(glyph.size.x, glyph.size.y, 1.f) * scale) * math::matrix4::translate(math::vec3(x_pos, y_pos, position.z)), color, glyph.rect }, font);

			x += scale * 0.25f + glyph.advance * scale;
		}
	}

	void Renderer::submitDrawTexture(Texture* const texture, const math::mat4& transform, const TextureRect& rect, const Color& color)
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
				m_textureRenderable.get(),
				m_spriteProgram.get(),
				m_viewProjectionMatrix,
				10000
			);
			m_commands.push_back(std::unique_ptr<RenderTextureCommand>(command));
		}

		command->push({ transform, color, rect }, texture);
	}

	void Renderer::submitDrawTexture(Texture* const texture, const math::vec3& position, const TextureRect& rect, const Color& color)
	{
		submitDrawTexture(texture, math::matrix4::translate(position), rect, color);
	}

	void Renderer::submitDrawTexture(Texture* const texture, const math::vec3& position, const float rotation, const TextureRect& rect, const Color& color)
	{
		submitDrawTexture(texture, math::matrix4::translate(position) * math::matrix4::rotate_z(rotation), rect, color);
	}

	void Renderer::submitDrawTexture(Texture* const texture, const math::vec3& position, const math::vec3& scale, const TextureRect& rect, const Color& color)
	{
		submitDrawTexture(texture, math::matrix4::scale(scale) * math::matrix4::translate(position), rect, color);
	}

	void Renderer::submitDrawTexture(Texture* const texture, const math::vec3& position, const float rotation, const math::vec3& scale, const TextureRect& rect, const Color& color)
	{
		submitDrawTexture(texture, math::matrix4::scale(scale) * math::matrix4::rotate_z(rotation) * math::matrix4::translate(position), rect, color);
	}
}