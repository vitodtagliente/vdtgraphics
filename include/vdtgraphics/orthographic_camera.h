/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include "camera.h"

namespace graphics
{
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera();
		OrthographicCamera(float near, float far);

		virtual math::matrix4 getViewMatrix() const override;
		virtual math::matrix4 getProjectionMatrix(int screenWidth, int screenHeight) const override;

		float nearPlane;
		float farPlane;
		bool pixelPerfect;
		int pixelsPerUnit;
	};
}