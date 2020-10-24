#include <vdtgraphics/renderer_2d.h>

#include <vdtgraphics/api.h>
#include <vdtgraphics/renderable.h>
#include <vdtgraphics/material.h>
#include <vdtgraphics/mesh.h>
#include <vdtgraphics/meshes/circle.h>
#include <vdtgraphics/meshes/quad.h>

namespace graphics
{
	Renderer2D::Renderer2D(API* const api)
		: Renderer(api)
		, m_circle(api->createRenderable(Circle{}))
		, m_quad(api->createRenderable(Quad{}))
	{		
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
		static Material* const material = m_api->getMaterialLibrary().get(Material::Default::Name::Color);

		if (material != nullptr)
		{
			Material* const materialInstance = material->createInstance();
			materialInstance->set(Material::Default::Property::Color, color);
			materialInstance->set(Material::Default::Property::ModelViewProjectionMatrix, transform);
			push(m_quad, materialInstance, transform);
		}
	}

	void Renderer2D::drawCircle(const Color& color, const vector2& position, const float radius)
	{
		static Material* const material = m_api->getMaterialLibrary().get(Material::Default::Name::Color);

		if (material != nullptr)
		{
			matrix4 transform = matrix4::scale(vec3(radius, radius, 0.0f)) * matrix4::translate(to_vec3(position));
			Material* const materialInstance = material->createInstance();
			materialInstance->set(Material::Default::Property::Color, color);
			materialInstance->set(Material::Default::Property::ModelViewProjectionMatrix, transform);
			push(m_circle, materialInstance, transform);
		}
	}

	void Renderer2D::drawTexture(Texture* const texture, const vector2& position)
	{
		static Material* const material = m_api->getMaterialLibrary().get(Material::Default::Name::Texture);

		if (material != nullptr)
		{
			matrix4 transform = matrix4::translate(to_vec3(position));
			Material* const materialInstance = material->createInstance();
			materialInstance->set(Material::Default::Property::Texture, texture);
			materialInstance->set(Material::Default::Property::ModelViewProjectionMatrix, transform);
			push(m_quad, materialInstance, transform);
		}
	}
}