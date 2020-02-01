/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "matrix4.h"
#include "vector3.h"
#include "quaternion.h"

namespace math
{
	class transform
	{
	public:

		transform();
		transform(const vector3& position, const quaternion& rotation, const vector3& scale);

		inline const matrix4& matrix() const { return m_matrix; }

		inline void update()
		{
			//m_matrix = matrix4::scale(scale) * rotation.matrix() * matrix4::translate(position);
			m_matrix = matrix4::scale(scale) * matrix4::rotate_z(rotation.z) * matrix4::translate(position);
		}

		vector3 position;
		quaternion rotation;
		vector3 scale;

	private:

		// cached matrix
		matrix4 m_matrix;
	};
}