/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "../mesh.h"

namespace graphics
{
	struct Circle : public Mesh
	{
	public:

		Circle(const unsigned int t_steps = 360);

	};
}