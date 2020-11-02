#include <vdtgraphics/renderable.h>

#include <vector>
#include <vdtgraphics/api.h>

namespace graphics
{
	//Renderable::Renderable(API* const api, const Mesh& mesh)
	//	: m_vertexBuffer()
	//	, m_indexBuffer()
	//{
	//	const std::vector<float>& vertices = mesh.getData();
	//	// m_vertexBuffer = api->createVertexBuffer(&vertices.front(), vertices.size(), sizeof(float) * vertices.size(), Mesh::VertexData::BufferLayout);
	//	// m_indexBuffer = api->createIndexBuffer(&mesh.indices.front(), mesh.indices.size());
	//}
	//
	//Renderable::Renderable(API* const api, const std::size_t vertices, const std::size_t indices)
	//	: m_vertexBuffer()
	//	, m_indexBuffer()
	//{
	//}

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