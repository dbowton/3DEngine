#pragma once
#include "Framework/System.h"
#include "Resource.h"
#include <string>
#include <map>
#include <memory>
#include <algorithm>
#include "Core/Utilities.h"

namespace dwb
{
	class ResourceSystem : public System
	{
	public:
		void StartUp() override {};
		void ShutDown() override {};
		void Update(float dt) override {};
		
		template<typename T>
		std::shared_ptr<T> Get(const std::string& name, void* data = nullptr);
		
		template <typename T>
		std::vector<std::shared_ptr<T>> Get();

		void Add(const std::string& name, std::shared_ptr<Resource> resource);

	private:
		std::map<std::string, std::shared_ptr<Resource>> resources;
	};

	template<typename T>
	inline std::shared_ptr<T> ResourceSystem::Get(const std::string& name, void* data)
	{
		if (resources.find(string_toLower(name)) != resources.end())
		{
			return std::dynamic_pointer_cast<T>(resources[string_toLower(name)]);
		}

		std::shared_ptr resource = std::make_shared<T>();
		resource->Load(name, data);
		resources[string_toLower(name)] = resource;

		return resource;
	}

	template <typename T>
	inline std::vector<std::shared_ptr<T>> ResourceSystem::Get()
	{
		std::vector<std::shared_ptr<T>> result;
		for (auto& element : resources)
		{
			// get the value of the map (first = key, second = value)
			// the value is a shared_ptr, get() the raw pointer and try to cast to type T*
			if (dynamic_cast<T*>(element.second.get()))
			{
				// if it is of type T, add the shared pointer to the vector
				result.push_back(std::dynamic_pointer_cast<T>(element.second));
			}
		}
		return result;
	}

	inline void ResourceSystem::Add(const std::string& name, std::shared_ptr<Resource> resource)
	{
		resources[string_toLower(name)] = resource;
	}
}