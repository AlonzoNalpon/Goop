/*!**********************************************************************
\file   ComponentArray.h
\author w.chinkitbryan\@digipen.edu
\date   6-September-2023
\brief  
  Custom array data structure to contain components
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved
*************************************************************************/
#pragma once
#include "../Entity/Entity.h"
#include <vector>
#include <unordered_map>

namespace GE
{
	namespace ECS
	{
		// Base class to allow for polymophic container
		// of ComponentArrays in ComponentManager without 
		// specifying template type
		class IComponentArray
		{
		public:
			virtual ~IComponentArray() = default;
			virtual void EntityDestroyed(const Entity& entity) = 0;
		};

		template <typename T>
		class ComponentArray : public IComponentArray
		{
		private:
			std::vector<T> m_components;
			std::unordered_map<Entity, size_t> m_entityToIndexMap;
			std::unordered_map<size_t, Entity> m_indexToEntityMap;

		public:
			ComponentArray() = default;
			~ComponentArray() = default;

			void Insert(const Entity& entity, const T& component);
			void Remove(const Entity& entity);
			T* GetData(const Entity& entity);
			T* GetData(const Entity& entity) const;
			void EntityDestroyed(const Entity& entity) override;
		};

#include "ComponentArray.tpp"
	}
}
