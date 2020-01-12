#include <vdtgraphics/renderer_2d.h>
#include <vdtgraphics/context.h>
#include <vdtgraphics/graphics_api.h>
#include <vdtgraphics/renderable.h>
#include <vdtgraphics/material.h>
#include <vdtgraphics/mesh.h>
#include <vdtgraphics/meshes/circle.h>
#include <vdtgraphics/meshes/quad.h>
#include <vdtgraphics/shader_library.h>
#include <vdtgraphics/shaders.h>

namespace graphics
{
	Renderer2D::Renderer2D(Context* const context)
		: Renderer(context)
		, m_circle(context->getAPI()->createRenderable(Circle{}))
		, m_quad(context->getAPI()->createRenderable(Quad{}))
	{		
	}

	void Renderer2D::initialize()
	{
		// initialize 2d default materials 
		if (ShaderLibrary* library = m_context->getAPI()->getShaderLibrary())
		{
			for (const std::string& name : { Shaders::names::ColorShader
				, Shaders::names::TextureShader
				, Shaders::names::CroppedTextureShader })
			{
				if (ShaderProgram* const program = library->get(name))
				{
					Material* const material = new Material(program);
					m_materialLibrary.add(name, material);
				}					
			}
		}		
	}

	void Renderer2D::drawRect(const Color& color, const vector2& position)
	{
		drawRect(
			color,
			matrix4::translate(to_vec3(position))
		);
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
		static Material* const material = getMaterialLibrary().get(Shaders::names::ColorShader);

		if (material != nullptr)
		{
			Material* const materialInstance = material->createInstance();
			materialInstance->set(Shaders::params::Color, color);
			materialInstance->set(Shaders::params::ModelViewProjectionMatrix, transform);
			push(m_quad, materialInstance, transform);
		}
	}

	void Renderer2D::drawCircle(const Color& color, const vector2& position, const float radius)
	{
		static Material* const material = getMaterialLibrary().get(Shaders::names::ColorShader);

		if (material != nullptr)
		{
			matrix4 transform = matrix4::scale(vec3(radius, radius, 0.0f)) * matrix4::translate(to_vec3(position));
			Material* const materialInstance = material->createInstance();
			materialInstance->set(Shaders::params::Color, color);
			materialInstance->set(Shaders::params::ModelViewProjectionMatrix, transform);
			push(m_circle, materialInstance, transform);
		}
	}
}