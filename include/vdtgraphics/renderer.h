/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <memory>
#include <stack>

#include <vdtmath/matrix4.h>
#include <vdtmath/vector3.h>

#include "common.h"
#include "color.h"
#include "render_command.h"
#include "texture_rect.h"

namespace graphics
{
	class Context;
	class Renderable;
	class ShaderLibrary;
	class RenderTarget;
	class ShaderProgram;
	class SpriteBatch;
	class Texture;

	class Renderer
	{
	public:
		Renderer() = default;
		virtual ~Renderer() = default;

		bool init(Context* const context);
		void uninit();

		void clear(const Color& color);
		void setViewport(int width, int height);
		void setWireframeMode(bool enabled);

		void setRenderTarget(RenderTarget* const renderTarget);

		void setProjectionMatrix(const math::matrix4& m);
		void setViewMatrix(const math::matrix4& m);
		const math::matrix4& getProjectionMatrix() const { return m_projectionMatrix; }
		const math::matrix4& getViewMatrix() const { return m_viewMatrix; }
		const math::matrix4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

		void submit(std::unique_ptr<RenderCommand> command);
		void submitDrawCircle(ShapeRenderStyle style, const math::vec3& position, float radius, const Color& color);
		void submitDrawLine(const math::vec3& point1, const Color& color1, const math::vec3& point2, const Color& color2);
		void submitDrawShape(ShapeRenderStyle style, const std::vector<Vertex>& vertices);
		void submitDrawRect(ShapeRenderStyle style, const math::vec3& position, float width, float height, const Color& color);
		void submitDrawTexture(Texture* const texture, const math::mat4& matrix, const TextureRect& rect = {}, const Color& color = Color::White);
		void submitDrawTexture(Texture* const texture, const math::vec3& position, const TextureRect& rect = {}, const Color& color = Color::White);
		void submitDrawTexture(Texture* const texture, const math::vec3& position, float rotation, const TextureRect& rect = {}, const Color& color = Color::White);
		void submitDrawTexture(Texture* const texture, const math::vec3& position, const math::vec3& scale, const TextureRect& rect = {}, const Color& color = Color::White);
		void submitDrawTexture(Texture* const texture, const math::vec3& position, float rotation, const math::vec3& scale, const TextureRect& rect = {}, const Color& color = Color::White);

		void flush();

	private:
		std::unique_ptr<ShaderProgram> createProgram(const std::string& name);

		std::vector<std::unique_ptr<RenderCommand>> m_commands;
		RenderTarget* m_renderTarget{ nullptr };
		std::unique_ptr<ShaderLibrary> m_shaderLibrary;
		// matrices
		math::mat4 m_projectionMatrix{ math::mat4::identity };
		math::mat4 m_viewMatrix{ math::mat4::identity };
		math::mat4 m_viewProjectionMatrix{ math::mat4::identity };
		// renderables
		std::unique_ptr<Renderable> m_shapeFillRenderable;
		std::unique_ptr<Renderable> m_shapeStrokeRenderable;
		std::unique_ptr<Renderable> m_textureRenderable;
		// programs
		std::unique_ptr<ShaderProgram> m_colorProgram;
		std::unique_ptr<ShaderProgram> m_shapeProgram;
		std::unique_ptr<ShaderProgram> m_spriteProgram;
		std::unique_ptr<ShaderProgram> m_textureProgram;
	};
}