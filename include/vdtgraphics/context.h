/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "color.h"

namespace graphics
{
	class API;

	class Context
	{
	public:

		enum class Type
		{
			Default
		};

		Context(API* const api, const Type type = Type::Default);
		virtual ~Context() = default;

		inline API* const getAPI() const { return m_api; }
		inline Type getType() const { return m_type; }

		virtual void enableAlpha(const bool bEnabled = true) = 0;
		virtual void clear(const Color& color) = 0;
		virtual void draw(const unsigned int vertices = 3) = 0;
		virtual void drawIndexed(const unsigned int numIndexes) = 0;
		virtual void setViewport(const int width, const int height) = 0;

	private:

		// graphics api
		API* m_api;
		// context type
		Type m_type;

	};
}