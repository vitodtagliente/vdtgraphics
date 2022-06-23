/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <vdtmath/transform.h>

#include "color.h"

namespace graphics
{
	class ParticleSystem
	{
	public:

		struct Particle
		{
			math::transform transform;
			math::vec3 direction;
			float life;
			float speed;
			Color color;
		};

	private:

	};
}