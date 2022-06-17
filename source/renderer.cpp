#include <vdtgraphics/renderer.h>
#include <vdtgraphics/shader.h>
#include <vdtgraphics/shader_program.h>

#include <map>

#include <glad/glad.h>

namespace graphics
{
	Renderer::Renderer(const int width, const int height, const Settings& settings)
		: m_width(width)
		, m_height(height)
		, m_clearColor(Color::Black)
		, m_fillPolygonBatch(settings.polygonBatchSize)
		, m_shaderLibrary()
		, m_strokePolygonBatch(settings.polygonBatchSize)
		, m_style(StyleType::fill)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::begin()
	{
		glViewport(0, 0, m_width, m_height);


	}

	void Renderer::flush()
	{
		glClearColor(m_clearColor.red, m_clearColor.green, m_clearColor.blue, m_clearColor.alpha);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);
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

	void Renderer::setStyle(const StyleType style)
	{
		m_style = style;
	}

	void Renderer::drawCircle(const math::vec3& position, float radius, const Color& color)
	{
	}

	void Renderer::drawLine(const math::vec3& p1, const Color& c1, const math::vec3& p2, const Color& c2)
	{
	}

	void Renderer::drawPoint(const math::vec3& position, const Color& color)
	{
	}

	void Renderer::drawPolygon(const std::vector<std::pair<math::vec3, Color>>& points)
	{
	}

	void Renderer::drawRect(const math::vec3& position, float width, float height, const Color& color)
	{
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
