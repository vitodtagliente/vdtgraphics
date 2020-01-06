#pragma once

#include <vector>
#include <vdtmath/vector.h>

using namespace math;

namespace graphics
{
	struct Mesh
	{
	public:

		struct VertexData
		{
			vector3 position;
			vector2 uv;

		};

		Mesh();
		Mesh(const std::vector<VertexData>& vertices);
		Mesh(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices);
		Mesh(const Mesh& mesh);
		virtual ~Mesh();

		void getData(std::vector<float>& data) const;

		Mesh& operator= (const Mesh& mesh);
		
		// vertex data
		std::vector<VertexData> vertices;
		// index data
		std::vector<unsigned int> indices;
	};
}