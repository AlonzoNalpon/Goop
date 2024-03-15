/*!**********************************************************************
\file   ComponentArray.h
\author w.chinkitbryan\@digipen.edu
\date   6-September-2023
\brief  
  Custom array data structure to contain components
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved
*************************************************************************/
#pragma once
#include "../Entity/Entity.h"
#include <vector>
#include <unordered_map>
#include <sstream>
#include <DebugTools/ErrorLogger/ErrorLogger.h>

namespace GE
{
	namespace ECS
	{
		// Forward declare to log component manager error
		class ComponentManager;

		// Base class to allow for polymophic container
		// of ComponentArrays in ComponentManager without 
		// specifying template type
		class IComponentArray
		{
		public:
			/*!*********************************************************************
			\brief
			  Defaulted constructor.
			************************************************************************/
			virtual ~IComponentArray() = default;
			/*!*********************************************************************
			\brief
			  Callback for when an entity is destroyed and should be removed from
				the component's index.

			\param entity
			  Entity that was removed
			************************************************************************/
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

			/*!*********************************************************************
			\brief
				Register a component to an entity.

			\param entity
				Entity which the component belongs to.

			\param component
			  Component which the entitiy has.
			************************************************************************/
			void Insert(const Entity& entity, const T& component);
			/*!*********************************************************************
			\brief
			  Removes the component belonging to entity from the array 

			\param entity
			  Entity's component that is being removed
			************************************************************************/
			void Remove(const Entity& entity);
			/*!*********************************************************************
			\brief
			  Returns the component of the entity.

			\param entity
			  Entity's component to get.

			\return
			  Entity's component.
			************************************************************************/
			T* GetData(const Entity& entity);
			/*!*********************************************************************
			\brief
				Const version of GetData(const& Entity).

			\param entity
				Entity's component to get.

			\return
				Entity's component.
			************************************************************************/
			T* GetData(const Entity& entity) const;
			/*!*********************************************************************
			\brief
			  Callback for when an entity is destroyed. Removes that entity's
				component from the system

			\param entity
			  Entity being destroyed
			************************************************************************/
			void EntityDestroyed(const Entity& entity) override;
		};

#include "ComponentArray.tpp"
	}
}
