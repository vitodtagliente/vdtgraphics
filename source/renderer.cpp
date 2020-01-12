#include <vdtgraphics/renderer.h>
#include <vdtgraphics/context.h>
#include <vdtgraphics/material.h>
#include <vdtgraphics/renderable.h>

namespace graphics
{
	Renderer::Renderer(Context* const context)
		: m_context(context)
		, m_materialLibrary()
		, m_textureLibrary()
		, m_commandBuffer()
		, m_drawingMode(DrawingMode::Fill)
	{
	}
	
	void Renderer::setDrawingMode(const DrawingMode drawingMode)
	{
		m_drawingMode = drawingMode;
	}
	
	void Renderer::push(Renderable* const renderable, Material* const material, const math::matrix4& transform)
	{
		m_commandBuffer.push_back(RenderCommand{ renderable, material, transform });
	}

	void Renderer::render()
	{
		for (const RenderCommand& command : m_commandBuffer)
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
				m_context->drawIndexed(static_cast<unsigned int>(mesh.indices.size()));
			}
			else
			{
				m_context->draw(static_cast<unsigned int>(mesh.vertices.size()));
			}
		}
		m_commandBuffer.clear();
	}

	void Renderer::clear(const Color& color)
	{
		m_context->clear(color);
	}
}