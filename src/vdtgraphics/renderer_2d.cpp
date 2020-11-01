#include <vdtgraphics/renderer_2d.h>

#include <vdtgraphics/api.h>
#include <vdtgraphics/buffer.h>
#include <vdtgraphics/renderable.h>
#include <vdtgraphics/material.h>
#include <vdtgraphics/mesh.h>
#include <vdtgraphics/meshes/circle.h>
#include <vdtgraphics/meshes/quad.h>

namespace graphics
{
	Renderer2D::Renderer2D(API* const api)
		: Renderer(api)
		, m_textureUnits(api->getTextureUnits())
		, m_batches()
	{
	}

	Renderer2D::~Renderer2D()
	{
		for (const BatchData& batch : m_batches)
		{
			delete batch.renderable;
		}
	}

	void Renderer2D::drawRect(const Color& color, const vector2& position)
	{
		drawRect(
			color,
			matrix4::translate(to_vec3(position))
		);
	}

	void Renderer2D::drawRect(const Color& color, const vector2& position, const vector2& scale)
	{
		drawRect(
			color,
			matrix4::scale(to_vec3(scale)) * matrix4::translate(to_vec3(position))
		);
	}

	void Renderer2D::drawRect(const Color& color, const vector2& position, const float theta)
	{
		drawRect(
			color,
			matrix4::rotate_z(theta) * matrix4::translate(to_vec3(position))
		);
	}

	void Renderer2D::drawRect(const Color& color, const vector2& position, const float theta, const vector2& scale)
	{
		drawRect(
			color,
			matrix4::scale(to_vec3(scale)) * matrix4::rotate_z(theta) * matrix4::translate(to_vec3(position))
		);
	}

	void Renderer2D::drawRect(const Color& color, const matrix4& transform)
	{
		static Material* const material = m_api->getMaterialLibrary().get(Material::Default::Name::Color);

		if (material != nullptr)
		{
			Material* const materialInstance = material->createInstance();
			materialInstance->set(Material::Default::Property::Color, color);
			materialInstance->set(Material::Default::Property::ModelViewProjectionMatrix, transform);
			//push(m_quad, materialInstance, transform);
		}
	}

	void Renderer2D::drawCircle(const Color& color, const vector2& position, const float radius)
	{
		static Material* const material = m_api->getMaterialLibrary().get(Material::Default::Name::Color);

		if (material != nullptr)
		{
			matrix4 transform = matrix4::scale(vec3(radius, radius, 0.0f)) * matrix4::translate(to_vec3(position));
			Material* const materialInstance = material->createInstance();
			materialInstance->set(Material::Default::Property::Color, color);
			materialInstance->set(Material::Default::Property::ModelViewProjectionMatrix, transform);
			//push(m_circle, materialInstance, transform);
		}
	}

	void Renderer2D::drawTexture(Texture* const texture, const vector2& position)
	{
		unsigned int textureIndex;
		BatchData& batch = findCandidateBatch(texture, textureIndex);
		addQuad(batch, position, Color::White, textureIndex);
	}

	void Renderer2D::render()
	{
		for (BatchData& batch : m_batches)
		{
			Renderable* const renderable = batch.renderable;
			renderable->getVertexBuffer()->set(&batch.mesh.vertices.front(), batch.mesh.vertices.size() * sizeof(float));
			renderable->getIndexBuffer()->set(&batch.mesh.indices.front(), batch.mesh.indices.size());
			batch.mesh.vertices.clear();
			batch.mesh.indices.clear();

			Material* materialInstance = m_api->getMaterialLibrary().get(Material::Default::Name::Texture)->createInstance();
			materialInstance->set(Material::Default::Property::Textures, batch.textures);
			materialInstance->set(Material::Default::Property::ModelViewProjectionMatrix, math::matrix4::identity);

			push(renderable, materialInstance, math::matrix4::identity);
		}

		Renderer::render();
	}

	Renderer2D::BatchData& Renderer2D::findCandidateBatch(Texture* const texture, unsigned int& textureIndex)
	{
		textureIndex = 0;
		if (m_batches.empty())
		{
			BatchData newBatch;
			static const unsigned int MaxQuads = 1000;
			newBatch.renderable = m_api->createRenderable(MaxQuads * 4, MaxQuads * 6);
			m_batches.push_back(newBatch);
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

	void Renderer2D::addQuad(BatchData& batch, const vector2& position, const Color& color, const unsigned int textureIndex)
	{
		const unsigned int start_index = static_cast<unsigned int>(batch.mesh.vertices.size());
		//  top right
		batch.mesh.vertices.push_back({ { position.x + 1.0f, position.y + 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }, textureIndex });
		// bottom right
		batch.mesh.vertices.push_back({ { position.x + 1.0f, position.y - 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0, 0.0f }, textureIndex });
		// bottom left
		batch.mesh.vertices.push_back({ { position.x - 1.0f, position.y - 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0, 0.0f }, textureIndex });
		// top left
		batch.mesh.vertices.push_back({ { position.x - 1.0f, position.y + 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0, 1.0f }, textureIndex });

		batch.mesh.indices.push_back(start_index + 0);
		batch.mesh.indices.push_back(start_index + 1);
		batch.mesh.indices.push_back(start_index + 3);
		batch.mesh.indices.push_back(start_index + 1);
		batch.mesh.indices.push_back(start_index + 2);
		batch.mesh.indices.push_back(start_index + 3);
	}
}