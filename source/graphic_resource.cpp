#include <vdtgraphics/graphic_resource.h>

namespace graphics
{
	ResourcePool ResourcePool::s_instance{};
	
	void ResourcePool::push(const GraphicResourceType type, void* ptr)
	{
		if (m_resources.find(type) == m_resources.end())
		{
			m_resources.insert({ type, {} });
		}
		m_resources[type].push_back(ptr);
	}

	void ResourcePool::refresh()
	{
		for (auto& pair : m_resources)
		{
			std::vector<void*>& pool = pair.second;
			for (auto it = pool.begin(); it != pool.end(); ++it)
			{
				if (*it == nullptr)
				{
					pool.erase(it);
				}
			}
		}
	}

	void ResourcePool::clear()
	{
		for (auto& pair : m_resources)
		{
			std::vector<void*>& pool = pair.second;
			for (auto it = pool.begin(); it != pool.end(); ++it)
			{
				if (*it == nullptr) continue;
				delete (*it);
			}
		}
	}
	
	ResourcePool& ResourcePool::getInstance()
	{
		return s_instance;
	}
	
	ResourcePool::~ResourcePool()
	{
		
	}
}