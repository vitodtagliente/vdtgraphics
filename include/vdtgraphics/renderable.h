/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "mesh.h"

namespace graphics
{
	class Renderable
	{
	public:

		Renderable();
		Renderable(const Mesh& mesh);
		virtual ~Renderable();

		virtual void bind() = 0;
		virtual void unbind() = 0;

		inline const Mesh& getMesh() const { return m_mesh; }

	private:

		// mesh
		Mesh m_mesh;
	};
}