#include <vdtgraphics/camera.h>

namespace graphics
{
	math::mat4 Camera::view(const math::transform& transform)
	{
		bool isInvertible = false;
		return transform.matrix().inverse(isInvertible);
	}

	math::mat4 Camera::view(math::transform& transform)
	{
		bool isInvertible = false;
		transform.update();
		return transform.matrix().inverse(isInvertible);
	}

	math::mat4 Camera::ortho(const float nearPlane, const float farPlane, const int screenWidth, const int screenHeight, const float aspectRatio)
	{
		const float w = static_cast<float>(screenWidth) * 0.5f * aspectRatio;
		const float h = static_cast<float>(screenHeight) * 0.5f;
		return math::mat4::orthographic(-w, w, -h, h, nearPlane, farPlane);
	}
}
