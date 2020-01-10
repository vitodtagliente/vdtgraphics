#include <vdtgraphics/meshes/circle.h>

namespace graphics
{
	Circle::Circle(const unsigned int steps)
		: Mesh()
	{
		float theta = 0.0f;
		const float step_amount = 360.0f / steps;
		// center vertex
		vertices.push_back({});
		vertices.push_back({ {cosf(theta), sinf(theta), 0.0f}, {0.0f, 0.0f} });
		for (unsigned int i = 1; i < steps; ++i)
		{
			vertices.push_back({ {cosf(theta), sinf(theta), 0.0f}, {0.0f, 0.0f} });
			indices.push_back(0);
			indices.push_back(static_cast<unsigned int>(vertices.size() - 1));
			indices.push_back(static_cast<unsigned int>(vertices.size()));
			theta += step_amount;
		}
	}
}