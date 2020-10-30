/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <map>
#include <string>
#include <vdtmath/matrix4.h>
#include "color.h"
#include "command_buffer.h"
#include "graphic_resource.h"
#include "material.h"
#include "resource_library.h"
#include "shader_program.h"

namespace graphics
{
	class API;
	class Renderable;

	class Renderer : public GraphicResource<GraphicResourceType::Renderer>
	{
	public:

		// dependency injection
		Renderer(API* const api);
		virtual ~Renderer() = default;

		virtual void initialize();

		// push render commands
		void push(Renderable* const renderable, Material* const material, const math::matrix4& transform = math::matrix4::identity);
		// render all pushed commands
		virtual void render();

		void clear(const Color& color);

	protected:

		// graphics API
		API* m_api;
		// command buffer
		CommandBuffer m_commandBuffer;
	};
}