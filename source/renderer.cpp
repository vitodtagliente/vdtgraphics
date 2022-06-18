#include <vdtgraphics/renderer.h>

#include <vdtgraphics/index_buffer.h>
#include <vdtgraphics/shader.h>
#include <vdtgraphics/shader_program.h>
#include <vdtgraphics/vertex_buffer.h>

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
		// , m_spriteBatch(settings.spriteBatchSize)
		, m_strokePolygonBatch(settings.polygonBatchSize)
		, m_projectionMatrix(math::mat4::identity)
		, m_viewMatrix(math::mat4::identity)
		, m_viewProjectionMatrix(math::mat4::identity)
		, m_polygonRenderable()
		, m_spriteBatchRenderable()
		, m_colorProgram(nullptr)
		, m_polygonProgram(nullptr)
		, m_spritebatchProgram(nullptr)
		, m_textureProgram(nullptr)
	{
	}

	void Renderer::init()
	{
		if (m_initialized) return;

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
			VertexBuffer& vb = *m_polygonRenderable.addVertexBuffer(Renderable::names::MainBuffer, 7 * 2000 * sizeof(float), BufferUsageMode::Static);
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

			VertexBuffer& vb = *m_spriteBatchRenderable.addVertexBuffer(Renderable::names::MainBuffer, sizeof(vertices), BufferUsageMode::Static);
			vb.fillData(vertices, sizeof(vertices));
			VertexBufferLayout& layout = vb.layout;
			layout.push(VertexBufferElement("position", VertexBufferElement::Type::Float, 3));
			layout.push(VertexBufferElement("coords", VertexBufferElement::Type::Float, 2));
			IndexBuffer& ib = *m_spriteBatchRenderable.addIndexBuffer(Renderable::names::MainBuffer, sizeof(indices), BufferUsageMode::Static);
			ib.fillData(indices, sizeof(indices));

			VertexBuffer& cropBuffer = *m_spriteBatchRenderable.addVertexBuffer("cropsBuffer", 4 * 2000 * sizeof(float), BufferUsageMode::Stream);
			cropBuffer.layout.push(VertexBufferElement("crop", VertexBufferElement::Type::Float, 4, true, true));
			cropBuffer.layout.startingIndex = 2;

			VertexBuffer& colorBuffer = *m_spriteBatchRenderable.addVertexBuffer("colorsBuffer", 4 * 2000 * sizeof(float), BufferUsageMode::Stream);
			colorBuffer.layout.push(VertexBufferElement("color", VertexBufferElement::Type::Float, 4, true, true));
			colorBuffer.layout.startingIndex = 3;

			VertexBuffer& transformBuffer = *m_spriteBatchRenderable.addVertexBuffer("transformsBuffer", 16 * 2000 * sizeof(float), BufferUsageMode::Stream);
			transformBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			transformBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			transformBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			transformBuffer.layout.push(VertexBufferElement("transform", VertexBufferElement::Type::Float, 4, true, true));
			transformBuffer.layout.startingIndex = 4;

			m_spriteBatchRenderable.bind();
		}
		// texture
		{
			// shaders
			m_textureProgram = createProgram(ShaderLibrary::names::TextureShader);
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::begin()
	{
		init();

		glViewport(0, 0, m_width, m_height);


	}

	void Renderer::flush()
	{
		glClearColor(m_clearColor.red, m_clearColor.green, m_clearColor.blue, m_clearColor.alpha);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		m_fillPolygonBatch.flush([this](const std::vector<float>& data) -> void
			{
				m_polygonRenderable.bind();

				VertexBuffer* vertexBuffer = m_polygonRenderable.findVertexBuffer(Renderable::names::MainBuffer);
				vertexBuffer->bind();
				vertexBuffer->fillData((void*)&data[0], data.size() * sizeof(float));

				m_polygonProgram->bind();
				m_polygonProgram->set("u_matrix", m_viewProjectionMatrix);

				const int primitiveType = GL_LINES;
				const int offset = 0;
				const int count = static_cast<int>(data.size() / 7);
				glDrawArrays(primitiveType, offset, count);
			}
		);
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
		if (m_style == StyleType::fill)
		{

		}
		else
		{

		}
	}

	void Renderer::drawLine(const math::vec3& p1, const Color& c1, const math::vec3& p2, const Color& c2)
	{
		if (m_style == StyleType::fill)
		{
			m_fillPolygonBatch.batch(p1, c1);
			m_fillPolygonBatch.batch(p2, c2);
		}
		else
		{
			m_strokePolygonBatch.batch(p1, c1);
			m_strokePolygonBatch.batch(p2, c2);
		}
	}

	void Renderer::drawPoint(const math::vec3& position, const Color& color)
	{
		if (m_style == StyleType::fill)
		{

		}
		else
		{

		}
	}

	void Renderer::drawPolygon(const std::vector<std::pair<math::vec3, Color>>& points)
	{
		if (m_style == StyleType::fill)
		{

		}
		else
		{

		}
	}

	void Renderer::drawRect(const math::vec3& position, float width, float height, const Color& color)
	{
		if (m_style == StyleType::fill)
		{

		}
		else
		{

		}
	}

	ShaderProgram* const Renderer::createProgram(const std::string& name)
	{
		std::map<Shader::Type, std::string> sources;
		auto it = m_shaderLibrary.getShaders().find(name);
		if (it != m_shaderLibrary.getShaders().end()
			&& Shader::Reader::parse(it->second, sources))
		{
			Shader vs(Shader::Type::Vertex, sources.find(Shader::Type::Vertex)->second);
			Shader fs(Shader::Type::Fragment, sources.find(Shader::Type::Fragment)->second);
			return new ShaderProgram({ &vs, &fs });
		}
		return nullptr;
	}


}
