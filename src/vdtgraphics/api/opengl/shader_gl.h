/// Copyright (c) Vito Domenico Tagliente

#pragma once 

#include <vdtgraphics/shader.h>

namespace graphics
{
	class ShaderGL final : public Shader
	{
	public:

		ShaderGL(const Type type, const std::string& source);
		virtual ~ShaderGL() override;
	};
}