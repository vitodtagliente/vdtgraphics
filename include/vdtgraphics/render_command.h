/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtmath/matrix4.h>

using namespace math;

namespace graphics
{
	// All render state required for submitting a render command.
	
	class Material;
	class Renderable;

	struct RenderCommand
	{
		Renderable* renderable;
		Material* material;
		matrix4 transform;

		RenderCommand()
			: renderable()
			, material()
			, transform(matrix4::identity)
		{

		}

		RenderCommand(
			Renderable* const renderable,
			Material* const material,
			const matrix4& transform = matrix4::identity
		)
			: renderable(renderable)
			, material(material)
			, transform(transform)
		{

		}
	};
}