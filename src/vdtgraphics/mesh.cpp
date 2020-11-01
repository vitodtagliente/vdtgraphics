#include <vdtgraphics/mesh.h>

namespace graphics
{
	const BufferLayout Mesh::VertexData::BufferLayout{
		BufferElement("position", BufferElement::Type::Float, 3, sizeof(float) * 3),
		BufferElement("color", BufferElement::Type::Float, 4, sizeof(float) * 4),
		BufferElement("texture_coords", BufferElement::Type::Float, 2, sizeof(float) * 2),
		BufferElement("texture_index", BufferElement::Type::Float, 1, sizeof(float) * 1)
	};

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
			// vertex color
			data.push_back(vertex_data.color.getRed());
			data.push_back(vertex_data.color.getGreen());
			data.push_back(vertex_data.color.getBlue());
			data.push_back(vertex_data.color.getAlpha());
			// vertex uv
			data.push_back(vertex_data.uv.x);
			data.push_back(vertex_data.uv.y);
			// texture index
			data.push_back(static_cast<float>(vertex_data.texture));
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