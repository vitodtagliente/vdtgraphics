/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "color.h"

namespace graphics
{
	class GraphicsAPI;

	class Context
	{
	public:

		enum class Type
		{
			Default
		};

		Context(GraphicsAPI* const api, const Type type = Type::Default);
		virtual ~Context() = default;

		inline GraphicsAPI* const getAPI() const { return m_api; }
		inline Type getType() const { return m_type; }

		virtual void enableAlpha(const bool bEnabled = true) = 0;
		virtual void clear(const Color& color) = 0;
		virtual void draw(const unsigned int vertices = 3) = 0;
		virtual void drawIndexed(const unsigned int numIndexes) = 0;
		virtual void setViewport(const int width, const int height) = 0;

	private:

		// graphics api
		GraphicsAPI* m_api;
		// context type
		Type m_type;

	};
}