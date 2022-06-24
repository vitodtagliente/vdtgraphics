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

	math::vec3 Camera::screenToWorldCoords(const math::vec2& screenCoords, const int screenWidth, const int screenHeight) const
	{
		math::vec4 normalizedScreenCoords(
			(screenCoords.x / static_cast<float>(screenWidth) - 0.5f) * 2.0f, // [-1,1]
			-(screenCoords.y / static_cast<float>(screenHeight) - 0.5f) * 2.0f, // [-1,1]
			.0f, // The near plane maps to Z=-1 in Normalized Device Coordinates
			1.0f
		);

		// bool isInvertible = false;
		// const math::vec4 viewCoords = m_viewProjectionMatrix.inverse(isInvertible) * normalizedScreenCoords;
		// const math::vec4 worldCoords = m_viewMatrix * math::vec4(viewCoords.x, viewCoords.y, .0f, 1.0f);
		// 
		// return math::vec3(worldCoords.x, worldCoords.y, worldCoords.z);

		bool isInvertible = false;
		const math::mat4 matrix = getProjectionMatrix(screenWidth, screenHeight).inverse(isInvertible);
		const math::mat4& viewMatrix = getViewMatrix();
		const math::vec4 worldCoords = viewMatrix * normalizedScreenCoords;

		return math::vec3(worldCoords.x - viewMatrix.m30, worldCoords.y - viewMatrix.m31, worldCoords.z);
	}

	void Camera::update()
	{
		transform.update();
	}
}
