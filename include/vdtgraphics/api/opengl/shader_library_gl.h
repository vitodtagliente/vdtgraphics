/// Copyright (c) Vito Domenico Tagliente

#pragma once 

#include <vdtgraphics/shader_library.h>

namespace graphics
{
	class ShaderLibraryGL : public ShaderLibrary
	{
	public:

		ShaderLibraryGL(API* const api);
		virtual ~ShaderLibraryGL() override = default;
	
	private:

		virtual const std::map<std::string, std::string>& getDefaultShaderSources() const;

	};
}