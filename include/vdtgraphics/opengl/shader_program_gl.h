/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <map>
#include <vdtgraphics/shader_program.h>

namespace graphics
{
	class ShaderProgramGL final : public ShaderProgram
	{
	public:

		ShaderProgramGL(const std::initializer_list<Shader*>& shaders);
		~ShaderProgramGL();

		virtual void bind() override;
		virtual void unbind() override;

		// uniform setters
		virtual void set(const std::string& name, const bool value) override;
		virtual void set(const std::string& name, const int value) override;
		virtual void set(const std::string& name, const float value) override;
		virtual void set(const std::string& name, const float* const matrix) override;
		virtual void set(const std::string& name, const float f1, const float f2, const float f3, const float f4) override;

	private:

		// find the uniform layout location
		int getUniformLocation(const std::string& name) const;

		// uniform locations
		mutable std::map<std::string, unsigned int> m_uniformLocations;
	};
}