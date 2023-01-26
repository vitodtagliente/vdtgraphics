/// Copyright (c) Vito Domenico Tagliente
#pragma once

namespace graphics
{
	struct RenderCommand
	{
		virtual ~RenderCommand() = default;

		virtual void execute() = 0;
	};
}