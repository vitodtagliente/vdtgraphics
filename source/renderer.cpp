#include <vdtgraphics/renderer.h>

#include <vdtgraphics/api.h>
#include <vdtgraphics/renderable.h>

namespace graphics
{
	Renderer::Renderer(API* const api)
		: m_api(api)
		, m_commandBuffer()
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
		for (RenderCommand& command : m_commandBuffer)
		{
			// bind the material
			// (activate shaders properties)
			command.material->bind();
			// bind the data to render
			Renderable* const renderable = command.renderable;
			renderable->bind();
			m_api->drawIndexed(static_cast<unsigned int>(renderable->getIndexBuffer()->getSize()));
			renderable->unbind();

			command.free();
		}
		m_commandBuffer.clear();
	}

	void Renderer::clear(const Color& color)
	{
		m_api->clear(color);
	}
}