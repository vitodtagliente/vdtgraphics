/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <string>
#include <vector>

#include <vdtmath/vector3.h>

#include "color.h"
#include "polygon_batch.h"
#include "shader_library.h"
#include "sprite_batch.h"

namespace graphics
{
	class ShaderProgram;

	class Renderer
	{
	public:
		struct Settings
		{
			size_t polygonBatchSize{ 2000 };
			size_t spriteBatchSize{ 2000 };
		};

		enum class StyleType
		{
			fill,
			stroke
		};

		Renderer(int width, int height, const Settings& settings = {});

		void begin();
		void flush();

		void setClearColor(const Color& color);
		const Color& getClearColor() const { return m_clearColor; }

		void setViewport(int width, int height);

		void setStyle(StyleType style);
		StyleType getStyle() const { return m_style; }

		void drawCircle(const math::vec3& position, float radius, const Color& color);
		void drawLine(const math::vec3& p1, const Color& c1, const math::vec3& p2, const Color& c2);
		void drawPoint(const math::vec3& position, const Color& color);
		void drawPolygon(const std::vector<std::pair<math::vec3, Color>>& points);
		void drawRect(const math::vec3& position, float width, float height, const Color& color);


	private:
		ShaderProgram* const createProgram(const std::string& name);

		int m_width, m_height;
		Color m_clearColor;
		PolygonBatch m_fillPolygonBatch;
		ShaderLibrary m_shaderLibrary;
		SpriteBatch m_spriteBatch;
		PolygonBatch m_strokePolygonBatch;
		StyleType m_style;
	};
}