#include <vdtgraphics/mesh.h>

namespace graphics
{
	Mesh::Mesh()
		: vertices()
		, indices()
	{
	
	}

	Mesh::Mesh(const std::vector<VertexData>& vertices)
		: vertices(vertices)
		, indices()
	{
	}

	Mesh::Mesh(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices)
		: vertices(vertices)
		, indices(indices)
	{
	}

	Mesh::Mesh(const Mesh& mesh)
	{
		vertices = mesh.vertices;
		indices = mesh.indices;
	}

	Mesh::~Mesh()
	{
	}

	std::vector<float> Mesh::getData() const
	{
		std::vector<float> data;

		for (const VertexData& vertex_data : vertices)
		{
			// vertex position
			data.push_back(vertex_data.position.x);
			data.push_back(vertex_data.position.y);
			data.push_back(vertex_data.position.z);
			// vertex uv
			data.push_back(vertex_data.uv.x);
			data.push_back(vertex_data.uv.y);
		}

		return data;
	}

	Mesh& Mesh::operator=(const Mesh& mesh)
	{
		vertices = mesh.vertices;
		indices = mesh.indices;
		return *this;
	}

}