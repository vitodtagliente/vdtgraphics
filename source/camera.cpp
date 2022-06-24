#include <vdtgraphics/camera.h>

namespace graphics
{
	math::matrix4 Camera::getViewMatrix() const
	{
		return transform.matrix();
	}

	math::matrix4 Camera::getProjectionMatrix(const int screenWidth, const int screenHeight) const
	{
		return math::matrix4::identity;
	}

	void Camera::update()
	{
		transform.update();
	}
}
