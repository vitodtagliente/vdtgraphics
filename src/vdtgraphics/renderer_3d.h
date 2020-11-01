/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "renderer.h"

namespace graphics
{
	class Renderer3D : public Renderer
	{
	public:

		Renderer3D(API* const api);
		virtual ~Renderer3D() = default;
	};

	/*
	class Renderer
	{
	public:

		enum class DrawingMode
		{
			Fill,
			Lines
		};

		// dependency injection
		Renderer(GraphicsAPI* const api);
		virtual ~Renderer();

		void initializeMaterials();
		virtual void enableAlpha(const bool bEnabled = true) = 0;
		virtual void clear(const Color& color) = 0;
		virtual void draw(const unsigned int vertices = 3) = 0;
		virtual void drawIndexed(const unsigned int vertices) = 0;
		virtual void setViewport(const int width, const int height) = 0;
		virtual void setDrawingMode(const DrawingMode drawingMode);
		
		void push(Renderable* const renderable, Material* const material, const matrix4& transform = matrix4::identity);
		// render all pushed commands
		void render();

		inline GraphicsAPI* const getAPI() const { return m_api; }
		inline MaterialLibrary* const getMaterialLibrary() const { return m_materialLibrary; }
		inline TextureLibrary* const getTextureLibrary() const { return m_textureLibrary; }

		// basic 2d drawing utilites
		void drawTexture(Texture* const texture, const vector2& position);
		void drawTexture(Texture* const texture, const vector2& position, const vector2& scale);
		void drawTexture(Texture* const texture, const vector2& position, const float theta);
		void drawTexture(Texture* const texture, const vector2& position, const float theta, const vector2& scale);
		void drawTexture(Texture* const texture, const matrix4& transform);
		// draw texture atlas
		void drawTextureCrop(Texture* const texture, const rect& rect, const vector2& position);
		void drawTextureCrop(Texture* const texture, const rect& rect, const vector2& position, const vector2& scale);
		void drawTextureCrop(Texture* const texture, const rect& rect, const vector2& position, const float theta);
		void drawTextureCrop(Texture* const texture, const rect& rect, const vector2& position, const float theta, const vector2& scale);
		void drawTextureCrop(Texture* const texture, const rect& rect, const matrix4& transform);
		// draw a rectangle 
		void drawRect(const Color& color, const vector2& position);
		void drawRect(const Color& color, const vector2& position, const vector2& scale);
		void drawRect(const Color& color, const vector2& position, const float theta);
		void drawRect(const Color& color, const vector2& position, const float theta, const vector2& scale);
		void drawRect(const Color& color, const matrix4& transform);
		// draw circle
		void drawCircle(const Color& color, const vector2& position, const float radius);

	protected:

		virtual const std::map<std::string, std::string>& getDefaultShaderSources() const = 0;

		// graphics api
		GraphicsAPI* m_api;
		// command buffer
		CommandBuffer m_commandBuffer;
		// material library
		MaterialLibrary* m_materialLibrary;
		// texture library
		TextureLibrary* m_textureLibrary;

		// renderable for sprites
		Renderable* m_quad;
		Renderable* m_circle;
		// drawing mode
		DrawingMode m_drawingMode;
	};
	*/
}