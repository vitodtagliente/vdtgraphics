#include <vdtgraphics/orthographic_camera.h>

namespace graphics
{
	OrthographicCamera::OrthographicCamera()
		: Camera()
		, nearPlane(-30.f)
		, farPlane(1000.f)
		, pixelPerfect(false)
		, pixelsPerUnit(32)
	{
	}

	OrthographicCamera::OrthographicCamera(const float near, const float far)
		: Camera()
		, nearPlane(near)
		, farPlane(far)
		, pixelPerfect(false)
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
		const float w = pixelPerfect ? static_cast<float>(screenWidth) / 2 / pixelsPerUnit : 1.f;
		const float h = pixelPerfect ? static_cast<float>(screenHeight) / 2 / pixelsPerUnit : 1.f;
		return math::mat4::orthographic(-w, w, -h, h, nearPlane, farPlane);
	}
}