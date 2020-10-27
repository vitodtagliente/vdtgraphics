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

		struct BatchData
		{
			Mesh mesh;
			std::vector<Texture*> textures;
		};

		SpriteBatch(const unsigned int textureUnits);

		void add(Texture* const texture, const vector2& position = vector2::zero);
		void clear();

		std::vector<BatchData>::iterator begin() { return m_batches.begin(); }
		std::vector<BatchData>::iterator end() { return m_batches.end(); }
		std::vector<BatchData>::const_iterator begin() const { return m_batches.begin(); }
		std::vector<BatchData>::const_iterator end() const { return m_batches.end(); }

		const std::vector<BatchData>& getBatches() const { return m_batches; }

	private:

		BatchData& findCandidateBatch(Texture* const texture, unsigned int& textureIndex);

		unsigned int m_textureUnits;
		std::vector<BatchData> m_batches;
		
	};
}