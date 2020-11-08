#include <vdtgraphics/renderer_2d.h>

#include <vdtgraphics/api.h>
#include <vdtgraphics/buffer.h>
#include <vdtgraphics/image.h>
#include <vdtgraphics/material.h>
#include <vdtgraphics/mesh.h>
#include <vdtgraphics/meshes/circle.h>
#include <vdtgraphics/meshes/quad.h>
#include <vdtgraphics/renderable.h>
#include <vdtgraphics/texture.h>

namespace graphics
{
	Renderer2D::Renderer2D(API* const api)
		: Renderer(api)
		, m_textureUnits(api->getTextureUnits())
		, m_batches()
		, m_whiteTexture()
	{
		Image whiteImage(Color::White, 1, 1);
		m_whiteTexture = api->createTexture(whiteImage);
	}

	Renderer2D::~Renderer2D()
	{
		for (const BatchData& batch : m_batches)
		{
			delete batch.renderable;
		}
		delete m_whiteTexture;
	}

	void Renderer2D::drawRect(const Color& color, const vector2& position)
	{
		unsigned int textureIndex;
		BatchData& batch = findCandidateBatch(m_whiteTexture, textureIndex);
		addQuad(batch, position, color, Texture::Coords{}, textureIndex);
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
			// push(m_circle, materialInstance, transform);
		}
	}

	void Renderer2D::drawTexture(Texture* const texture, const vector2& position)
	{
		unsigned int textureIndex;
		BatchData& batch = findCandidateBatch(texture, textureIndex);
		addQuad(batch, position, Color::White, Texture::Coords{}, textureIndex);
	}

	void Renderer2D::drawTexture(Texture* const texture, const Texture::Coords& texCoords, const vector2& position)
	{
		unsigned int textureIndex;
		BatchData& batch = findCandidateBatch(texture, textureIndex);
		addQuad(batch, position, Color::White, texCoords, textureIndex);
	}

	void Renderer2D::render()
	{
		for (BatchData& batch : m_batches)
		{
			Renderable* renderable = batch.renderable;
			const std::vector<float>& vertices = batch.mesh.getData();
			renderable->getVertexBuffer()->update(&vertices.front(), static_cast<unsigned int>(batch.mesh.vertices.size()));
			renderable->getIndexBuffer()->update(&batch.mesh.indices.front(), static_cast<unsigned int>(batch.mesh.indices.size()));

			batch.mesh.vertices.clear();
			batch.mesh.indices.clear();

			Material* materialInstance = m_api->getMaterialLibrary().get(Material::Default::Name::Spritebatch)->createInstance();
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
			static const unsigned int MaxQuads = 1;
			VertexBuffer* const vb = m_api->createVertexBuffer(sizeof(Mesh::VertexData), MaxQuads * 4, BufferUsageMode::Dynamic, PrimitiveType::Triangles);
			vb->update(Mesh::VertexData::BufferLayout);
			IndexBuffer* const ib = m_api->createIndexBuffer(MaxQuads * 6, BufferUsageMode::Dynamic);
			newBatch.renderable = m_api->createRenderable(vb, ib);
			newBatch.textures.push_back(texture);

			m_batches.push_back(newBatch);
			return m_batches.front();
		}

		BatchData& batch = m_batches[0];
		for (int i = 0; i < batch.textures.size(); ++i)
		{
			if (texture == batch.textures[i])
			{
				textureIndex = i;
				return batch;
			}
		}

		textureIndex = static_cast<unsigned int>(batch.textures.size());
		batch.textures.push_back(texture);
		return batch;
	}

	void Renderer2D::addQuad(BatchData& batch, const vector2& position, const Color& color, const Texture::Coords& texCoords, const unsigned int textureIndex)
	{
		const unsigned int start_index = static_cast<unsigned int>(batch.mesh.vertices.size());

		const vector2& max = texCoords.getMin() + texCoords.getMax();
		//  top right
		batch.mesh.vertices.push_back({ { position.x + 1.0f, position.y + 1.0f, 0.0f }, color, max, textureIndex });
		// bottom right
		batch.mesh.vertices.push_back({ { position.x + 1.0f, position.y - 1.0f, 0.0f }, color, { max.x, texCoords.getMin().y }, textureIndex });
		// bottom left
		batch.mesh.vertices.push_back({ { position.x - 1.0f, position.y - 1.0f, 0.0f }, color, texCoords.getMin(), textureIndex });
		// top left
		batch.mesh.vertices.push_back({ { position.x - 1.0f, position.y + 1.0f, 0.0f }, color, { texCoords.getMin().x, max.y }, textureIndex });

		batch.mesh.indices.push_back(start_index + 0);
		batch.mesh.indices.push_back(start_index + 1);
		batch.mesh.indices.push_back(start_index + 3);
		batch.mesh.indices.push_back(start_index + 1);
		batch.mesh.indices.push_back(start_index + 2);
		batch.mesh.indices.push_back(start_index + 3);
	}
}