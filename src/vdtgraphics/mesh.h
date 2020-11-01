#pragma once

#include <vector>
#include <vdtmath/vector.h>
#include "buffer.h"
#include "color.h"

using namespace math;

namespace graphics
{
	struct Mesh
	{
	public:

		struct VertexData
		{
			vector3 position;
			Color color;
			vector2 uv;
			unsigned int texture;

			static const BufferLayout BufferLayout;
		};

		Mesh();
		Mesh(const std::vector<VertexData>& vertices);
		Mesh(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices);
		Mesh(const Mesh& mesh);
		virtual ~Mesh();

		std::vector<float> getData() const;

		Mesh& operator= (const Mesh& mesh);
		
		// vertex data
		std::vector<VertexData> vertices;
		// index data
		std::vector<unsigned int> indices;
	};
}