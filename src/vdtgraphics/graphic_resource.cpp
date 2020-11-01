#include <vdtgraphics/graphic_resource.h>

namespace graphics
{
	ResourcePool ResourcePool::s_instance{};
	
	void ResourcePool::insert(const GraphicResourceType type, void* ptr)
	{
		if (m_resources.find(type) == m_resources.end())
		{
			m_resources.insert({ type, {} });
		}
		m_resources[type].push_back(ptr);
	}

	void ResourcePool::erase(const GraphicResourceType type, void* ptr)
	{
		if (m_resources.find(type) != m_resources.end())
		{
			std::vector<void*>& pool = m_resources[type];
			const auto& it = std::find_if(pool.begin(), pool.end(), [ptr](const void* el) {
				return el == ptr;
			});
			if (it != pool.end())
			{
				pool.erase(it);
			}
		}
	}

	void ResourcePool::refresh()
	{

	}

	void ResourcePool::clear()
	{
		m_resources.clear();
	}
	
	ResourcePool& ResourcePool::getInstance()
	{
		return s_instance;
	}
	
	ResourcePool::~ResourcePool()
	{
		
	}
}