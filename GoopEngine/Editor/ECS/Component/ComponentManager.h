#pragma once
#include "ComponentArray.h"
#include <unordered_map>

namespace GE
{
	namespace ECS
	{
		// Component signature is a bitflagm which is just an unsigned int
		// therefore ComponentType is the same basic data type
		using ComponentType = unsigned int;

		class ComponentManager
		{
		public:
			ComponentManager() = default;
			~ComponentManager();

			template <typename T>
			ComponentType GetComponentType();

			template <typename T>
			void AddComponent(Entity& entity, const T& component);

			template <typename T>
			void RemoveComponent(Entity& entity);

			template <typename T>
			T* GetComponent(const Entity& entity);

			void EntityDestroyed(Entity& entity);

		private:
			// Use unordered map here as const char* is not a C++ string
			// and thus does not overload < operator to be used in STL map
			std::unordered_map<const char*, ComponentType> m_componentTypes;
			std::unordered_map<const char*, IComponentArray*> m_componentArrays;

			ComponentType m_nextComponentType{0};

			template <typename T>
			ComponentArray<T>* GetComponentArray();

			template <typename T>
			void RegisterComponent();
		};

#include "ComponentManager.tpp"
	}
}
