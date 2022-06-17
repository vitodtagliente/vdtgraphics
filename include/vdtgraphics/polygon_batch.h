/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <vector>
#include <vdtmath/vector3.h>

#include "color.h"

namespace graphics
{
	class PolygonBatch
	{
	public:
		PolygonBatch(size_t batchSize);

		void batch(const math::vec3& position, const Color& color);
		void flush(class Context* const context);

	private:

		class Batch
		{
		public:
			Batch(size_t size);

			inline bool empty() const { return count == 0; }
			inline bool full() const { return count >= size; }

			void batch(const math::vec3& position, const Color& color);
			void clear();

			size_t count;
			std::vector<float> data;
			size_t size;
		};

		Batch& findNextBatch();

		size_t m_batchSize;
		size_t m_batchIndex;
		std::vector<Batch> m_batches;
	};
}