#include <vdtgraphics/renderer.h>

#include <vdtgraphics/index_buffer.h>
#include <vdtgraphics/shader.h>
#include <vdtgraphics/shader_program.h>
#include <vdtgraphics/texture.h>
#include <vdtgraphics/vertex_buffer.h>

#include <initializer_list>
#include <map>

#include <glad/glad.h>

namespace graphics
{
	Renderer::Renderer(const int width, const int height, const Settings& settings)
		: m_width(width)
		, m_height(height)
		, m_initialized(false)
		, m_clearColor(Color::Black)
		, m_shaderLibrary()
		, m_style(StyleType::fill)
		, m_fillPolygonBatch(settings.polygonBatchSize)
		, m_spriteBatch(settings.spriteBatchSize)
		, m_strokePolygonBatch(settings.polygonBatchSize)
		, m_projectionMatrix(math::mat4::identity)
		, m_viewMatrix(math::mat4::identity)
		, m_viewProjectionMatrix(math::mat4::identity)
		, m_polygonRenderable(nullptr)
		, m_spriteBatchRenderable(nullptr)
		, m_colorProgram(nullptr)
		, m_polygonProgram(nullptr)
		, m_spritebatchProgram(nullptr)
		, m_textureProgram(nullptr)
	{
	}

	void Renderer::init()
	{
		if (m_initialized) return;

		if (!gladLoadGL())
		{
			return;
		}

		// color
		{
			// shaders
			m_colorProgram = createProgram(ShaderLibrary::names::ColorShader);
		}
		// gizmos
		{
			// shaders
			m_polygonProgram = createProgram(ShaderLibrary::names::GizmosShader);

			// render data
			m_polygonRenderable = std::make_unique< Renderable>();
			VertexBuffer& vb = *m_polygonRenderable->addVertexBuffer(Renderable::names::MainBuffer, 7 * 2000 * sizeof(float), BufferUsageMode::Static);
			VertexBufferLayout& layout = vb.layout;
			layout.push(VertexBufferElement("position", VertexBufferElement::Type::Float, 3));
			layout.push(VertexBufferElement("color", VertexBufferElement::Type::Float, 4));
		}
		// spritebatch
		{
			// shaders
			m_spritebatchProgram = createProgram(ShaderLibrary::names::SpriteBatchShader);

			float vertices[] = {
				/*
				 First pixel from memory is bottom-left for OpenGL.
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f
				*/
				 1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
				-1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 1.0f
			};

			unsigned int indices[] = {
				0, 1, 3, 1, 2, 3
			};

			m_spriteBatchRenderable = std::make_unique< Renderable>();
			VertexBuffer& vb = *m_spriteBatchRenderable->addVertexBuffer(Renderable::names::MainBuffer, sizeof(vertices), BufferUsageMode::Static);
			vb.fillData(vertices, sizeof(vertices));
			VertexBufferLayout& layout = vb.layout;
			layout.push(VertexBufferElement("position", VertexBufferElement::Type::Float, 3));
			layout.push(VertexBufferElement("coords", VertexBufferElement::Type::Float, 2));
			IndexBuffer& ib = *m_spriteBatchRenderable->addIndexBuffer(Renderable::names::MainBuffer, sizeof(indices), BufferUsageMode::Static);
			ib.fillData(indices, sizeof(indices));

			VertexBuffer& cropBuffer = *m_spriteBatchRenderable->addVertexBuffer("cropsBuffer", 4 * 2000 * sizeof(float), BufferUsageMode::Stream);
			cropBuffer.layout.push(VertexBufferElement("crop", VertexBufferElement::Type::Float, 4, true, true));
			cropBuffer.layout.startingIndex = 2;

			VertexBuffer& colorBuffer = *m_spriteBatchRenderable->addVertexBuffer("colorsBuffer", 4 * 2000 * sizeof(float), BufferUsageMode::Stream);
			colorBuffer.layout.push(VertexBufferElement("color", VertexBufferElement::Type::Float, 4, true, true));
			colorBuffer.layout.startingIndex = 3;

			VertexBuffer& transformBuffer = *m_spriteBatchRenderable->addVertexBuffer("transformsBuffer", 16 * 2000 * sizeof(float), BufferUsageMode::Stream);
			transformBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			transformBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			transformBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			transformBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			transformBuffer.layout.startingIndex = 4;

			m_spriteBatchRenderable->bind();
		}
		// texture
		{
			// shaders
			m_textureProgram = createProgram(ShaderLibrary::names::TextureShader);
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_initialized = true;
	}

	void Renderer::begin()
	{
		init();

		if (!m_initialized) return;

		glViewport(0, 0, m_width, m_height);


	}

	int Renderer::flush()
	{
		if (!m_initialized) return 0;

		int drawCalls = 0;

		glClearColor(m_clearColor.red, m_clearColor.green, m_clearColor.blue, m_clearColor.alpha);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		m_spriteBatch.flush([this, &drawCalls](Texture* const texture, const std::vector<float>& transforms, const std::vector<float>& rects, const std::vector<float>& colors) -> void
			{
				const size_t instances = transforms.size() / 16;
				if (instances != rects.size() / 4) return;

				m_spriteBatchRenderable->bind();

				VertexBuffer& cropBuffer = *m_spriteBatchRenderable->findVertexBuffer("cropsBuffer");
				cropBuffer.bind();
				cropBuffer.fillData((void*)&rects[0], rects.size() * sizeof(float));

				VertexBuffer& colorBuffer = *m_spriteBatchRenderable->findVertexBuffer("colorsBuffer");
				colorBuffer.bind();
				colorBuffer.fillData((void*)&colors[0], colors.size() * sizeof(float));

				VertexBuffer& transformBuffer = *m_spriteBatchRenderable->findVertexBuffer("transformsBuffer");
				transformBuffer.bind();
				transformBuffer.fillData((void*)&transforms[0], transforms.size() * sizeof(float));

				m_spritebatchProgram->bind();
				texture->bind(0);
				m_spritebatchProgram->set("u_texture", 0);
				m_spritebatchProgram->set("u_matrix", m_viewProjectionMatrix);

				const int primitiveType = GL_TRIANGLES;
				const int offset = 0;
				const int count = 6;
				const int numInstances = static_cast<int>(instances);
				const int indexType = GL_UNSIGNED_INT;

				++drawCalls;
				glDrawElementsInstanced(primitiveType, count, indexType, offset, numInstances);
			}
		);

		m_fillPolygonBatch.flush([this, &drawCalls](const std::vector<float>& data) -> void
			{
				m_polygonRenderable->bind();

				VertexBuffer* vertexBuffer = m_polygonRenderable->findVertexBuffer(Renderable::names::MainBuffer);
				vertexBuffer->bind();
				vertexBuffer->fillData((void*)&data[0], data.size() * sizeof(float));

				m_polygonProgram->bind();
				m_polygonProgram->set("u_matrix", m_viewProjectionMatrix);

				const int primitiveType = GL_TRIANGLES;
				const int offset = 0;
				const int count = static_cast<int>(data.size() / 7);

				++drawCalls;
				glDrawArrays(primitiveType, offset, count);
			}
		);

		m_strokePolygonBatch.flush([this, &drawCalls](const std::vector<float>& data) -> void
			{
				m_polygonRenderable->bind();

				VertexBuffer* vertexBuffer = m_polygonRenderable->findVertexBuffer(Renderable::names::MainBuffer);
				vertexBuffer->bind();
				vertexBuffer->fillData((void*)&data[0], data.size() * sizeof(float));

				m_polygonProgram->bind();
				m_polygonProgram->set("u_matrix", m_viewProjectionMatrix);

				const int primitiveType = GL_LINES;
				const int offset = 0;
				const int count = static_cast<int>(data.size() / 7);

				++drawCalls;
				glDrawArrays(primitiveType, offset, count);
			}
		);

		return drawCalls;
	}

	void Renderer::setClearColor(const Color& color)
	{
		m_clearColor = color;
	}

	void Renderer::setViewport(const int width, const int height)
	{
		m_width = width;
		m_height = height;
	}

	void Renderer::setProjectionMatrix(const math::matrix4& m)
	{
		m_projectionMatrix = m;
	}

	void Renderer::setViewMatrix(const math::matrix4& m)
	{
		m_viewMatrix = m;
	}

	void Renderer::setStyle(const StyleType style)
	{
		m_style = style;
	}

	void Renderer::drawCircle(const math::vec3& position, float radius, const Color& color)
	{		
		const float accuracy = 100.f * radius;
		const float step = (2 * math::pi) / accuracy;
		float angle = 0.0f;
		for (int i = 0; i < accuracy; ++i)
		{
			if (m_style == StyleType::fill)
			{
				m_fillPolygonBatch.batch(position.x, color);
				m_fillPolygonBatch.batch(position + math::vec3(radius * std::sin(angle), radius * std::cos(angle), 0.f), color);
				m_fillPolygonBatch.batch(position + math::vec3(radius * std::sin(angle + step), radius * std::cos(angle + step), 0.f), color);
			}
			else
			{
				m_strokePolygonBatch.batch(position + math::vec3(radius * std::sin(angle), radius * std::cos(angle), 0.f), color);
				m_strokePolygonBatch.batch(position + math::vec3(radius * std::sin(angle + step), radius * std::cos(angle + step), 0.f), color);
			}
			angle += step;
		}
	}

	void Renderer::drawLine(const math::vec3& point1, const math::vec3& point2, const Color& color)
	{
		m_strokePolygonBatch.batch(point1, color);
		m_strokePolygonBatch.batch(point2, color);
	}

	void Renderer::drawPolygon(const std::vector<std::pair<math::vec3, Color>>& points)
	{
		PolygonBatch& batch = m_style == StyleType::fill
			? m_fillPolygonBatch
			: m_strokePolygonBatch;

		for (const std::pair<math::vec3, Color>& pair : points)
		{
			batch.batch(pair.first, pair.second);
		}
	}

	void Renderer::drawRect(const math::vec3& position, float width, float height, const Color& color)
	{
		const float w = width / 2;
		const float h = height / 2;

		if (m_style == StyleType::fill)
		{
			m_fillPolygonBatch.batch(position + math::vec3(w, h, 0.f), color);
			m_fillPolygonBatch.batch(position + math::vec3(-w, h, 0.f), color);
			m_fillPolygonBatch.batch(position + math::vec3(-w, -h, 0.f), color);
			m_fillPolygonBatch.batch(position + math::vec3(-w, -h, 0.f), color);
			m_fillPolygonBatch.batch(position + math::vec3(w, -h, 0.f), color);
			m_fillPolygonBatch.batch(position + math::vec3(w, h, 0.f), color);
		}
		else
		{
			m_strokePolygonBatch.batch(position + math::vec3(w, h, 0.f), color);
			m_strokePolygonBatch.batch(position + math::vec3(-w, h, 0.f), color);
			m_strokePolygonBatch.batch(position + math::vec3(-w, h, 0.f), color);
			m_strokePolygonBatch.batch(position + math::vec3(-w, -h, 0.f), color);
			m_strokePolygonBatch.batch(position + math::vec3(-w, -h, 0.f), color);
			m_strokePolygonBatch.batch(position + math::vec3(w, -h, 0.f), color);
			m_strokePolygonBatch.batch(position + math::vec3(w, -h, 0.f), color);
			m_strokePolygonBatch.batch(position + math::vec3(w, h, 0.f), color);
		}		
	}

	void Renderer::drawTexture(Texture* const texture, const math::mat4& matrix, const TextureRect& rect, const Color& color)
	{
		m_spriteBatch.batch(texture, matrix, rect, color);
	}

	std::unique_ptr<ShaderProgram> Renderer::createProgram(const std::string& name)
	{
		std::map<Shader::Type, std::string> sources;
		auto it = m_shaderLibrary.getShaders().find(name);
		if (it != m_shaderLibrary.getShaders().end()
			&& Shader::Reader::parse(it->second, sources))
		{
			Shader vs(Shader::Type::Vertex, sources.find(Shader::Type::Vertex)->second);
			Shader fs(Shader::Type::Fragment, sources.find(Shader::Type::Fragment)->second);
			return std::make_unique<ShaderProgram>(std::initializer_list<Shader*>{ &vs, & fs });
		}
		return nullptr;
	}


}
