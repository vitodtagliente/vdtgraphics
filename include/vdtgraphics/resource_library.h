/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <map>
#include <string>

namespace graphics
{
	template <typename T>
	class ResourceLibrary
	{
	public:

		ResourceLibrary() : m_resources() {};
		virtual ~ResourceLibrary()
		{
			clear();
		}

		void add(const std::string& name, T* const resource)
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

		void remove(const std::string& name)
		{
			const auto it = m_resources.find(name);
			if (it != m_resources.end())
			{
				delete it->second;
				m_resources.erase(it);
			}
		}

		T* const get(const std::string& name) const
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
		std::map<std::string, T*> m_resources;

	};
}