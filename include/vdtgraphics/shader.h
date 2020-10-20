/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <map>
#include <string>

namespace graphics
{
	class Shader
	{
	public:

		typedef unsigned int id_t;

		enum class Type
		{
			Invalid = 0,
			Vertex,
			Fragment
		};

		enum class State
		{
			Unknown,
			Compiled,
			Error,
			Unloaded
		};

		class Reader final
		{
		public:
			// read the file content
			static bool load(const std::string& filename, std::string& source);
			// parse different shader sources from the same source
			static bool parse(const std::string& content, std::map<Type, std::string>& sources);
		};

		Shader(const Type type, const std::string& source);
		Shader(const Shader&) = delete;
		virtual ~Shader() = default;

		inline id_t getId() const { return m_id; }
		inline Type getType() const { return m_type; }
		inline State getState() const { return m_state; }
		inline const std::string& getErrorMessage() const { return m_errorMessage; }
		inline bool isValid() const { return m_id != INVALID_ID; }
		inline operator bool() const { return isValid(); }

		static std::string to_string(const Type type);
		static Type to_type(const std::string& str);

		Shader& operator=(const Shader& shader) = delete;

		bool operator==(const Shader& shader) const
		{
			return m_id == shader.getId();
		}

		bool operator!=(const Shader& shader) const
		{
			return m_id != shader.getId();
		}

		static constexpr id_t INVALID_ID = 0;

	protected:

		// shader id
		id_t m_id;
		// shader type
		Type m_type;
		// shader state
		State m_state;
		// store compile errors
		std::string m_errorMessage;
	};
}