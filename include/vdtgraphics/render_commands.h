/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <vector>

#include <vdtmath/matrix4.h>

#include "common.h"
#include "render_command.h"

namespace graphics
{
	class Renderable;
	class ShaderProgram;
	class Texture;

	class RenderShapeCommand final : public RenderCommand
	{
	public:
		RenderShapeCommand(Renderable* const renderable, ShaderProgram* const program, const math::mat4& viewProjectionMatrix, ShapeRenderStyle style, size_t capacity);

		size_t capacity() const { return m_capacity; }
		size_t size() const { return m_size; }
		bool hasCapacity(const size_t numOfVertices) const { return m_capacity - m_size >= numOfVertices; }

		ShapeRenderStyle getStyle() const { return m_style; }

		void push(const Vertex& vertex);
		
		virtual void execute() override;

	private:
		size_t m_capacity;
		std::vector<float> m_data;
		ShaderProgram* m_program;
		Renderable* m_renderable;
		size_t m_size;
		ShapeRenderStyle m_style;
		math::mat4 m_viewProjectionMatrix;
	};

	class RenderTextureCommand : public RenderCommand
	{
	public:
		RenderTextureCommand(Renderable* const renderable, ShaderProgram* const program, Texture* const texture, const math::mat4& viewProjectionMatrix, size_t capacity);

		size_t capacity() const { return m_capacity; }
		size_t size() const { return m_size; }
		bool hasCapacity(const size_t numOfTextures) const { return m_capacity - m_size >= numOfTextures; }

		Texture* const getTexture() const { return m_texture; }

		void push(const SpriteVertex& vertex);

		virtual void execute() override;

	private:
		size_t m_capacity; 
		std::vector<float> m_data;
		ShaderProgram* m_program;
		Renderable* m_renderable;
		size_t m_size;
		Texture* m_texture;
		math::mat4 m_viewProjectionMatrix;
	};
}