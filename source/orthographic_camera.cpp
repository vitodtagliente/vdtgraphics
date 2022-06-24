#include <vdtgraphics/orthographic_camera.h>

namespace graphics
{
	OrthographicCamera::OrthographicCamera()
		: Camera()
		, nearPlane(-1.0f)
		, farPlane(1.0f)
		, pixelPerfect(true)
		, pixelsPerUnit(32)
	{
	}

	OrthographicCamera::OrthographicCamera(const float near, const float far)
		: Camera()
		, nearPlane(near)
		, farPlane(far)
		, pixelPerfect(true)
		, pixelsPerUnit(32)
	{
	}

	math::matrix4 OrthographicCamera::getViewMatrix() const
	{
		bool isInvertible = false;
		return transform.matrix().inverse(isInvertible);
	}

	math::matrix4 OrthographicCamera::getProjectionMatrix(const int screenWidth, const int screenHeight) const
	{
		float w = static_cast<float>(screenWidth) / 2;
		float h = static_cast<float>(screenHeight) / 2;
		if (pixelPerfect)
		{
			w /= pixelsPerUnit;
			h /= pixelsPerUnit;
		}
		return math::mat4::orthographic(-w, w, -h, h, nearPlane, farPlane);
	}

	math::vec3 OrthographicCamera::screenToWorldCoords(const math::vec2& screenCoords, int screenWidth, int screenHeight) const
	{
		math::vec3 worldCoords = Camera::screenToWorldCoords(screenCoords, screenWidth, screenHeight);
		float w = static_cast<float>(screenWidth) / 2;
		float h = static_cast<float>(screenHeight) / 2;
		if (pixelPerfect)
		{
			w /= pixelsPerUnit;
			h /= pixelsPerUnit;
		}
		return math::vec3(worldCoords.x * w, worldCoords.y * h, .0f);
	}
}