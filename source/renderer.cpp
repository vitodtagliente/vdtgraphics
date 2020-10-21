#include <vdtgraphics/renderer.h>

#include <vdtgraphics/api.h>
#include <vdtgraphics/material.h>
#include <vdtgraphics/renderable.h>

namespace graphics
{
	Renderer::Renderer(API* const api)
		: m_api(api)
		, m_materialLibrary()
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
			command.renderable->bind();
			// render the command
			const Mesh& mesh = command.renderable->getMesh();
			if (mesh.indices.size() != 0)
			{
				m_api->drawIndexed(static_cast<unsigned int>(mesh.indices.size()));
			}
			else
			{
				m_api->draw(static_cast<unsigned int>(mesh.vertices.size()));
			}

			command.free();
		}
		m_commandBuffer.clear();
	}

	void Renderer::clear(const Color& color)
	{
		m_api->clear(color);
	}
}