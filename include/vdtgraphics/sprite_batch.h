/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vector>
#include "mesh.h"
#include "render_command.h"
#include "texture.h"
#include <vdtmath/vector.h>

using namespace math;

namespace graphics
{
	class API;
	class Renderer2D;

	class SpriteBatch
	{
	public:

		SpriteBatch(const unsigned int textureUnits);

		void add(Texture* const texture);
		void render(API* const api, Renderer2D* const renderer);

	private:

		struct BatchData
		{
			std::vector<Mesh::VertexData> vertices;
			std::vector<unsigned int> indices;
			std::vector<Texture*> textures;
		};

		BatchData& findCandidateBatch(Texture* const texture, unsigned int& textureIndex);

		unsigned int m_textureUnits;
		std::vector<BatchData> m_batches;
		
	};
}