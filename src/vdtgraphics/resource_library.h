/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <map>
#include <string>

namespace graphics
{
	template <typename V, typename K = std::string>
	class ResourceLibrary
	{
	public:

		ResourceLibrary() : m_resources() {};
		virtual ~ResourceLibrary()
		{
			clear();
		}

		void add(const K& name, V* const resource)
		{
			const auto it = m_resources.find(name);
			if (it == m_resources.end())
			{
				m_resources.insert({ name, resource });
			}
			else
			{
				it->second = resource;
			}
		}

		void remove(const K& name)
		{
			const auto it = m_resources.find(name);
			if (it != m_resources.end())
			{
				delete it->second;
				m_resources.erase(it);
			}
		}

		V* const get(const K& name) const
		{
			const auto it = m_resources.find(name);
			if (it != m_resources.end())
			{
				return it->second;
			}
			return nullptr;
		}

		void clear() 
		{
			for (const auto& pair : m_resources)
			{
				delete pair.second;
			}
			m_resources.clear();
		}

		size_t size() const 
		{
			return m_resources.size();
		}

	private:

		// cached textures
		std::map<K, V*> m_resources;

	};
}