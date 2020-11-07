#include <vdtgraphics/renderer.h>

#include <vdtgraphics/api.h>
#include <vdtgraphics/renderable.h>

namespace graphics
{
	Renderer::Renderer(API* const api)
		: m_api(api)
		, m_commandBuffer()
		, m_stats()
	{
	}

	void Renderer::initialize()
	{
	}
	
	void Renderer::push(Renderable* const renderable, Material* const material, const math::matrix4& transform)
	{
		m_commandBuffer.push_back(RenderCommand{ renderable, material, transform });
	}

	void Renderer::render()
	{
		m_stats.clear();

		for (RenderCommand& command : m_commandBuffer)
		{
			command.material->bind();
			m_api->draw(command.renderable);

			command.free();

			++m_stats.drawCalls;
		}

		m_commandBuffer.clear();
	}

	void Renderer::clear(const Color& color)
	{
		m_api->clear(color);
	}
	
	void Renderer::Stats::clear()
	{
		drawCalls = 0;
	}
}