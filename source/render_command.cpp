#include <vdtgraphics/render_command.h>

#include <vdtgraphics/material.h>
#include <vdtgraphics/renderable.h>

namespace graphics
{
	RenderCommand::RenderCommand()
		: renderable()
		, material()
		, transform(matrix4::identity)
	{

	}

	RenderCommand::RenderCommand(Renderable* const renderable, Material* const material, const matrix4& transform)
		: renderable(renderable)
		, material(material)
		, transform(transform)
	{

	}

	RenderCommand::~RenderCommand()
	{

	}
	
	void RenderCommand::free()
	{
		if (renderable->oneTimeRendering)
		{
			delete renderable;
		}

		if (material->isInstance())
		{
			delete material;
		}
	}
}