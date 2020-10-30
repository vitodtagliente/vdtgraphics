/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "renderer.h"
#include <vdtmath/vector2.h>
#include <vdtmath/matrix4.h>
#include "mesh.h"

namespace graphics
{
	class API;
	class VertexBuffer;
	class IndexBuffer;
	class Texture;

	class Renderer2D : public Renderer
	{
	public:

		Renderer2D(API* const api);
		virtual ~Renderer2D() = default;

		// draw a rectangle 
		void drawRect(const Color& color, const vector2& position);
		void drawRect(const Color& color, const vector2& position, const vector2& scale);
		void drawRect(const Color& color, const vector2& position, const float theta);
		void drawRect(const Color& color, const vector2& position, const float theta, const vector2& scale);
		void drawRect(const Color& color, const matrix4& transform);
		// draw circle
		void drawCircle(const Color& color, const vector2& position, const float radius);

		void drawTexture(Texture* const texture, const vector2& position);

		virtual void render() override;

	private:

		struct BatchData
		{
			Renderable* renderable;
			Mesh mesh;
			std::vector<Texture*> textures;
		};

		BatchData& findCandidateBatch(Texture* const texture, unsigned int& textureIndex);
		void addQuad(BatchData& batch, const vector2& position, const Color& color, const unsigned int textureIndex);

		unsigned int m_textureUnits;
		std::vector<BatchData> m_batches;
	};
}