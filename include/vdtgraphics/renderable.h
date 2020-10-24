/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "mesh.h"

namespace graphics
{
	class API;

	class Renderable
	{
	public:

		Renderable(const Mesh& mesh);
		virtual ~Renderable();

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void render(API* const api);

		inline unsigned int getVerticesCount() const { return m_vertices; }
		inline unsigned int getIndicesCount() const { return m_indices; }

	private:

		unsigned int m_vertices;
		unsigned int m_indices;
	};
}