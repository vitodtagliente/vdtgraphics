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
	
	void SpriteBatch::add(Texture* const texture)
	{
		unsigned int texture_index;
		BatchData& batch = findCandidateBatch(texture, texture_index);

		const unsigned int start_index = static_cast<unsigned int>(batch.vertices.size());
		//  top right
		batch.vertices.push_back({ { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }, texture_index });
		// bottom right
		batch.vertices.push_back({ { 1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0, 0.0f }, texture_index });
		// bottom left
		batch.vertices.push_back({ { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0, 0.0f }, texture_index });
		// top left
		batch.vertices.push_back({ { -1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0, 1.0f }, texture_index });

		batch.indices.push_back(start_index + 0);
		batch.indices.push_back(start_index + 1);
		batch.indices.push_back(start_index + 3);
		batch.indices.push_back(start_index + 1);
		batch.indices.push_back(start_index + 2);
		batch.indices.push_back(start_index + 3);
	}

	void SpriteBatch::render(API* const api, Renderer2D* const renderer)
	{
		for (const auto& batch : m_batches)
		{
			Mesh mesh(batch.vertices, batch.indices);
			Renderable* renderable = api->createRenderable(mesh);
			renderable->oneTimeRendering = true;
			Material* materialInstance = api->getMaterialLibrary().get(Material::Default::Name::Texture)->createInstance();
			
			materialInstance->set(Material::Default::Property::Textures, batch.textures);
			
			materialInstance->set(Material::Default::Property::ModelViewProjectionMatrix, math::matrix4::identity);
			renderer->push(renderable, materialInstance, math::matrix4::identity);
		}
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