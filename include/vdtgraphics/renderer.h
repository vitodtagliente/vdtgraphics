/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtmath/matrix4.h>
#include "color.h"
#include "command_buffer.h"
#include "material_library.h"
#include "texture_library.h"

namespace graphics
{
	class Context;
	class Renderable;

	class Renderer
	{
	public:

		enum class DrawingMode
		{
			Fill,
			Lines
		};

		// dependency injection
		Renderer(Context* const context);
		virtual ~Renderer() = default;

		virtual void initialize();

		inline Context* const getContext() const { return m_context; }
		inline MaterialLibrary& getMaterialLibrary() { return m_materialLibrary; }
		inline TextureLibrary& getTextureLibrary() { return m_textureLibrary; }

		virtual void setDrawingMode(const DrawingMode drawingMode);
		inline DrawingMode getDrawingMode() const { return m_drawingMode; }

		// push render commands
		void push(Renderable* const renderable, Material* const material, const math::matrix4& transform = math::matrix4::identity);
		// render all pushed commands
		virtual void render();

		void clear(const Color& color);

	protected:

		// graphics context
		Context* m_context;
		// material library
		MaterialLibrary m_materialLibrary;
		// texture library
		TextureLibrary m_textureLibrary;
		// command buffer
		CommandBuffer m_commandBuffer;
		// drawing mode
		DrawingMode m_drawingMode;
	};
}