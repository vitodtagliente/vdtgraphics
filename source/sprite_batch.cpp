#include <vdtgraphics/sprite_batch.h>

#include <vdtgraphics/api.h>
#include <vdtgraphics/renderer_2d.h>
#include <vdtgraphics/renderable.h>
#include <vdtmath/matrix.h>

namespace graphics
{
	SpriteBatch::SpriteBatch(const unsigned int textureUnits)
		: m_textureUnits(textureUnits)
		, m_batches()
	{
	}
	
	void SpriteBatch::add(Texture* const texture, const vector2& position)
	{
		unsigned int texture_index;
		BatchData& batch = findCandidateBatch(texture, texture_index);

		const unsigned int start_index = static_cast<unsigned int>(batch.mesh.vertices.size());
		//  top right
		batch.mesh.vertices.push_back({ { position.x + 1.0f, position.y + 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }, texture_index });
		// bottom right
		batch.mesh.vertices.push_back({ { position.x + 1.0f, position.y - 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0, 0.0f }, texture_index });
		// bottom left
		batch.mesh.vertices.push_back({ { position.x - 1.0f, position.y - 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0, 0.0f }, texture_index });
		// top left
		batch.mesh.vertices.push_back({ { position.x - 1.0f, position.y + 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0, 1.0f }, texture_index });

		batch.mesh.indices.push_back(start_index + 0);
		batch.mesh.indices.push_back(start_index + 1);
		batch.mesh.indices.push_back(start_index + 3);
		batch.mesh.indices.push_back(start_index + 1);
		batch.mesh.indices.push_back(start_index + 2);
		batch.mesh.indices.push_back(start_index + 3);
	}

	void SpriteBatch::clear()
	{
		m_batches.clear();
	}

	SpriteBatch::BatchData& SpriteBatch::findCandidateBatch(Texture* const texture, unsigned int& textureIndex)
	{
		textureIndex = 0;
		if (m_batches.empty())
		{
			m_batches.push_back({});
			m_batches[0].textures.push_back(texture);
			return m_batches[0];
		}

		// TODO
		BatchData& batch = m_batches[0];
		for (int i = 0; i < batch.textures.size(); ++i)
		{
			if (texture == batch.textures[i])
			{
				textureIndex = i;
				return batch;
			}
		}

		textureIndex = batch.textures.size();
		batch.textures.push_back(texture);
		return batch;
	}
}