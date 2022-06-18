/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <functional>
#include <list>
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
		void flush(const std::function<void(const std::vector<float>& data)>& handler);

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
		std::list<Batch> m_batches;
		std::list<Batch>::iterator m_batchIterator;
	};
}