/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <vdtmath/matrix4.h>
#include <vdtmath/vector3.h>

#include "color.h"
#include "polygon_batch.h"
#include "renderable.h"
#include "shader_library.h"
#include "shader_program.h"
#include "sprite_batch.h"

namespace graphics
{
	class Renderer2D
	{
	public:
		struct Settings
		{
			size_t polygonBatchSize{ 2000 };
			size_t spriteBatchSize{ 2000 };
		};

		Renderer2D(int width, int height, const Settings& settings = {});

		void init();

		void begin();
		int flush();

		void setClearColor(const Color& color);
		const Color& getClearColor() const { return m_clearColor; }

		void setViewport(int width, int height);

		void setProjectionMatrix(const math::matrix4& m);
		void setViewMatrix(const math::matrix4& m);
		const math::matrix4& getProjectionMatrix() const { return m_projectionMatrix; }
		const math::matrix4& getViewMatrix() const { return m_viewMatrix; }
		const math::matrix4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

		void setPolygonStyle(PolygonStyle style);
		PolygonStyle getPolygonStyle() const { return m_polygonStyle; }

		void drawCircle(const math::vec3& position, float radius, const Color& color);
		void drawLine(const math::vec3& point1, const math::vec3& point2, const Color& color);
		void drawPolygon(const std::vector<std::pair<math::vec3, Color>>& points);
		void drawRect(const math::vec3& position, float width, float height, const Color& color);
		void drawTexture(Texture* const texture, const math::mat4& matrix, const TextureRect& rect = {}, const Color& color = Color::White);
		void drawTexture(Texture* const texture, const math::vec3& position, const TextureRect& rect = {}, const Color& color = Color::White);
		void drawTexture(Texture* const texture, const math::vec3& position, float rotation, const TextureRect& rect = {}, const Color& color = Color::White);
		void drawTexture(Texture* const texture, const math::vec3& position, const math::vec3& scale, const TextureRect& rect = {}, const Color& color = Color::White);
		void drawTexture(Texture* const texture, const math::vec3& position, float rotation, const math::vec3& scale, const TextureRect& rect = {}, const Color& color = Color::White);

	private:
		std::unique_ptr<ShaderProgram> createProgram(const std::string& name);

		int m_width;
		int m_height;
		bool m_initialized;
		Color m_clearColor;
		ShaderLibrary m_shaderLibrary;
		PolygonStyle m_polygonStyle;
		// Batches
		PolygonBatch m_fillPolygonBatch;
		SpriteBatch m_spriteBatch;
		PolygonBatch m_strokePolygonBatch;
		// Matrices
		math::mat4 m_projectionMatrix;
		math::mat4 m_viewMatrix;
		math::mat4 m_viewProjectionMatrix;
		// Renderables
		std::unique_ptr<Renderable> m_polygonRenderable;
		std::unique_ptr<Renderable> m_spriteBatchRenderable;
		// Programs
		std::unique_ptr<ShaderProgram> m_colorProgram;
		std::unique_ptr<ShaderProgram> m_polygonProgram;
		std::unique_ptr<ShaderProgram> m_spritebatchProgram;
	};
}