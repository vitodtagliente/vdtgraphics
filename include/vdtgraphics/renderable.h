/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "buffer.h"
#include "graphic_resource.h"
#include "mesh.h"

namespace graphics
{
	class API;

	class Renderable : public GraphicResource<GraphicResourceType::Renderable>
	{
	public:

		Renderable(API* const api, const Mesh& mesh);
		Renderable(VertexBuffer* const vertexBuffer, IndexBuffer* const indexBuffer);
		virtual ~Renderable();

		virtual void bind() = 0;
		virtual void unbind() = 0;

		inline VertexBuffer* const getVertexBuffer() const { return m_vertexBuffer; }
		inline IndexBuffer* const getIndexBuffer() const { return m_indexBuffer; }

	protected:

		VertexBuffer* m_vertexBuffer;
		IndexBuffer* m_indexBuffer;
	};
}