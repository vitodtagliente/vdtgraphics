#include <vdtgraphics/renderable.h>

#include <vector>
#include <vdtgraphics/api.h>

namespace graphics
{
	Renderable::Renderable(API* const api, const Mesh& mesh)
		: m_vertexBuffer()
		, m_indexBuffer()
	{
		const std::vector<float>& vertices = mesh.getData();
		m_vertexBuffer = api->createVertexBuffer(&vertices.front(), sizeof(float) * vertices.size());
		m_indexBuffer = api->createIndexBuffer(&mesh.indices.front(), mesh.indices.size());

		m_vertexBuffer->layout.push(BufferElement("position", BufferElement::Type::Float, 3, sizeof(float) * 3));
		m_vertexBuffer->layout.push(BufferElement("color", BufferElement::Type::Float, 4, sizeof(float) * 4));
		m_vertexBuffer->layout.push(BufferElement("texture_coords", BufferElement::Type::Float, 2, sizeof(float) * 2));
		m_vertexBuffer->layout.push(BufferElement("texture_index", BufferElement::Type::Float, 1, sizeof(float) * 1));
	}

	Renderable::Renderable(VertexBuffer* const vertexBuffer, IndexBuffer* const indexBuffer)
		: m_vertexBuffer(vertexBuffer)
		, m_indexBuffer(indexBuffer)
	{
	}

	Renderable::~Renderable()
	{
		delete m_vertexBuffer;
		delete m_indexBuffer;
	}
}