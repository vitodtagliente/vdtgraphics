/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <vdtmath/matrix4.h>
#include "color.h"
#include "command_buffer.h"
#include "material_library.h"

namespace graphics
{
	class API;
	class Renderable;

	class Renderer
	{
	public:

		// dependency injection
		Renderer(API* const api);
		virtual ~Renderer() = default;

		virtual void initialize();

		inline API* const getAPI() const { return m_api; }
		inline MaterialLibrary& getMaterialLibrary() { return m_materialLibrary; }

		// push render commands
		void push(Renderable* const renderable, Material* const material, const math::matrix4& transform = math::matrix4::identity);
		// render all pushed commands
		virtual void render();

		void clear(const Color& color);

	protected:

		// graphics API
		API* m_api;
		// material library
		MaterialLibrary m_materialLibrary;
		// command buffer
		CommandBuffer m_commandBuffer;
	};
}