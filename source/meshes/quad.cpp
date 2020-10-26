#include <vdtgraphics/meshes/quad.h>

namespace graphics
{
	Quad::Quad()
		: Mesh()
	{
		// top right
		vertices.push_back({ { 1.0f, 1.0f, 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0, 1.0f}, 0 });
		// bottom right
		vertices.push_back({ { 1.0f, -1.0f, 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0, 0.0f}, 0 });
		// bottom left
		vertices.push_back({ { -1.0f, -1.0f, 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0, 0.0f}, 0 });
		// top left
		vertices.push_back({ { -1.0f, 1.0f, 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0, 1.0f}, 0 });

		indices = { 0, 1, 3, 1, 2, 3 };
	}
}