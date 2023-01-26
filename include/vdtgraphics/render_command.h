/// Copyright (c) Vito Domenico Tagliente
#pragma once

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

		virtual RenderCommandResult execute() = 0;
	};
}