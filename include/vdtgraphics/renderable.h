/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "buffer.h"
#include "mesh.h"

namespace graphics
{
	class API;

	class Renderable
	{
	public:

		Renderable(API* const api, const Mesh& mesh);
		Renderable(VertexBuffer* const vertexBuffer, IndexBuffer* const indexBuffer);
		virtual ~Renderable();

		virtual void bind() = 0;
		virtual void unbind() = 0;

		inline VertexBuffer* const getVertexBuffer() const { return m_vertexBuffer; }
		inline IndexBuffer* const getIndexBuffer() const { return m_indexBuffer; }

		virtual void render(API* const api);

		bool oneTimeRendering = false;

	protected:

		VertexBuffer* m_vertexBuffer;
		IndexBuffer* m_indexBuffer;
	};
}