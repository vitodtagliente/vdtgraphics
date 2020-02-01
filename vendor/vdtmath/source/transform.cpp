#include <vdtmath/transform.h>

namespace math
{
	transform::transform()
		: position()
		, rotation()
		, scale(1.0f, 1.0f, 1.0f)
		, m_matrix(matrix4::identity)
	{

	}

	transform::transform(const vector3& position, const quaternion& rotation, const vector3& scale)
		: position(position)
		, rotation(rotation)
		, scale(scale)
		, m_matrix(matrix4::identity)
	{

	}
}