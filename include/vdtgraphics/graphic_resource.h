/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <cstddef>
#include <map>
#include <vector>

namespace graphics
{
	enum class GraphicResourceType
	{
		IndexBuffer,
		Material,
		Renderable,
		Renderer,
		Shader,
		ShaderProgram,
		Texture,
		VertexBuffer
	};

	template <GraphicResourceType T>
	class GraphicResource
	{
	public:

		virtual ~GraphicResource();

		static void* operator new(std::size_t size);
	};

	class ResourcePool
	{
	public:

		template <GraphicResourceType T>
		friend class GraphicResource;
		friend class API;

		inline const auto& getResource() const { return m_resources; }

		void insert(const GraphicResourceType type, void* ptr);
		void erase(const GraphicResourceType type, void* ptr);
		void refresh();
		void clear();

		static ResourcePool& getInstance();

	private:

		ResourcePool() = default;
		~ResourcePool();

		std::map<GraphicResourceType, std::vector<void*>> m_resources;
		static ResourcePool s_instance;
	};

	template<GraphicResourceType T>
	inline GraphicResource<T>::~GraphicResource()
	{
		ResourcePool& pool = ResourcePool::getInstance();
		pool.erase(T, this);
	}

	template <GraphicResourceType T>
	void* GraphicResource<T>::operator new(std::size_t size)
	{
		void* ptr = ::operator new(size);
		ResourcePool& pool = ResourcePool::getInstance();
		pool.insert(T, ptr);
		return ptr;
	}
}