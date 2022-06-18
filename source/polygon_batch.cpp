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
		, m_batches()
		, m_batchIterator(m_batches.begin())
	{
		// start with one batch at least
		m_batches.push_back(Batch(batchSize));
	}

	void PolygonBatch::batch(const math::vec3& position, const Color& color)
	{
		findNextBatch().batch(position, color);
	}

	void PolygonBatch::flush(const std::function<void(const std::vector<float>& data)>& handler)
	{
		for (auto it = m_batches.begin(); it != m_batches.end(); ++it)
		{
			if (it->empty()) continue;

			handler(it->data);
			it->clear();
		}
		m_batchIterator = m_batches.begin();
	}

	PolygonBatch::Batch& PolygonBatch::findNextBatch()
	{
		if (!m_batchIterator->full())
		{
			return *m_batchIterator;
		}

		++m_batchIterator;
		if (m_batchIterator == m_batches.end())
		{
			m_batches.push_back(Batch(m_batchSize));
			m_batchIterator = m_batches.end();
			--m_batchIterator;
		}
		return *m_batchIterator;
	}
}