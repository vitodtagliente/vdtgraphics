/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <vdtmath/matrix4.h>

namespace graphics
{
	enum class RenderCommandResult
	{
		Invalid,
		OK
	};

	struct RenderCommand
	{
		virtual ~RenderCommand() = default;

		virtual RenderCommandResult execute(const math::mat4& viewProjectionMatrix) = 0;
	};
}