#include <vdtgraphics/polygon_batch.h>

namespace graphics
{
#define VERTICE_COMPONENTS 7

	PolygonBatch::Batch::Batch(size_t size)
		: count(0)
		, data()
		, size(size)
	{
		data.reserve(size * VERTICE_COMPONENTS);
	}

	void PolygonBatch::Batch::batch(const math::vec3& position, const Color& color)
	{
		data.insert(data.end(), { position.x, position.y, position.z, color.red, color.green, color.blue, color.alpha });
		++count;
	}

	void PolygonBatch::Batch::clear()
	{
		count = 0;
		data.clear();
	}

	PolygonBatch::PolygonBatch(const size_t batchSize)
		: m_batchSize(batchSize)
		, m_batchIndex()
		, m_batches()
	{
		for (int i = 0; i < 10; ++i)
		{
			m_batches.push_back(Batch(batchSize));
		}
	}

	void PolygonBatch::batch(const math::vec3& position, const Color& color)
	{
		findNextBatch().batch(position, color);
	}

	void PolygonBatch::flush(Context* const context)
	{
		for (auto it = m_batches.begin(); it != m_batches.end(); ++it)
		{
			if (it->empty()) continue;

			context->drawLines(it->data);
			it->clear();
		}
		m_batchIndex = 0;
	}

	PolygonBatch::Batch& PolygonBatch::findNextBatch()
	{
		if (!m_batches[m_batchIndex].full())
		{
			return m_batches[m_batchIndex];
		}

		++m_batchIndex;
		if (m_batchIndex >= m_batches.size())
		{
			m_batches.push_back(Batch(m_batchSize));
		}
		return m_batches[m_batchIndex];
	}
}