/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <initializer_list>
#include <string>
#include <vector>
#include "graphic_resource.h"

namespace graphics
{
	class API;
	class Shader;

	class ShaderProgram : public GraphicResource<GraphicResourceType::ShaderProgram>
	{
	public:

		typedef unsigned int id_t;

		enum class State
		{
			Unknown,
			Error,
			Linked
		};

		ShaderProgram(const std::initializer_list<Shader*>& shaders);
		ShaderProgram(const ShaderProgram&) = delete;
		virtual ~ShaderProgram() = default;

		static ShaderProgram* const parse(API* const api, const std::string& source);

		virtual void bind() = 0;
		virtual void unbind() = 0;

		inline id_t getId() const { return m_id; }
		inline State getState() const { return m_state; }
		inline const std::string& getErrorMessage() const { return m_errorMessage; }
		inline bool isValid() const { return m_id != INVALID_ID; }
		inline operator bool() const { return m_id != INVALID_ID; }

		ShaderProgram& operator=(const ShaderProgram&) = delete;

		inline bool operator==(const ShaderProgram& program)
		{
			return m_id == program.getId();
		}

		inline bool operator!=(const ShaderProgram& program)
		{
			return m_id != program.getId();
		}

		// uniform setters
		virtual void set(const std::string& name, const bool value) = 0;
		virtual void set(const std::string& name, const int value) = 0;
		virtual void set(const std::string& name, const float value) = 0;
		virtual void set(const std::string& name, const float* const matrix) = 0;
		virtual void set(const std::string& name, const std::vector<int>& value) = 0;
		virtual void set(const std::string& name, const float f1, const float f2, const float f3, const float f4) = 0;

		static constexpr id_t INVALID_ID = 0;

	protected:

		// program id
		id_t m_id;
		// state
		State m_state;
		// error message
		std::string m_errorMessage;
	};
}