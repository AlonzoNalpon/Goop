/*!*********************************************************************
\file   ComponentManager.h
\author w.chinkitbryan\@digipen.edu
\date   9-September-2023
\brief  
  Container and manager for all arrays of components
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "ComponentArray.h"
#include <unordered_map>
#include <sstream>
#include "../../Debugger/ErrorLogger/ErrorLogger.h"

namespace GE
{
	namespace ECS
	{
		// Component signature is a bitflag which is just an unsigned int
		// therefore ComponentType is the same basic data type
		// Functionally the same as ComponentSignature
		using ComponentType = unsigned int;

		class ComponentManager
		{
		public:
			/*!*********************************************************************
			\brief
			  Default constructor.
			************************************************************************/
			ComponentManager() = default;

			/*!*********************************************************************
			\brief
			  Default destructor.
			************************************************************************/
			~ComponentManager();

			/*!*********************************************************************
			\brief
			  Get the individual true bit of the component as ComponentSignature
				(unsigned int). If component does not exist, it will be registered.
			  
			\return
			  ComponentSignature.
			************************************************************************/
			template <typename T>
			ComponentType GetComponentSignature();

			/*!*********************************************************************
			\brief
			  Creates an array of Components.			  
			************************************************************************/
			template <typename T>
			void RegisterComponent();

			/*!******************************************************************
			\brief
			  Adds a component to its own component array and registers an entity
				to have that component.

			\param entity
				Entity which component is being added to.

			\param component
				Component that is added.
			********************************************************************/
			template <typename T>
			void AddComponent(Entity& entity, const T& component);

			/*!******************************************************************
			\brief
			  Removes a component from an entity.

			\param entity
				Entity which component is being removed from
			********************************************************************/
			template <typename T>
			void RemoveComponent(Entity& entity);

			/*!******************************************************************
			\brief
			  Returns a component from an entity.

			\param entity
				Entity to get component from

			\return 
				Pointer to entity's component.
				Returns null if component does not exist or is inactive.
			********************************************************************/
			template <typename T>
			T* GetComponent(const Entity& entity);

			/*!*********************************************************************
			\brief
			  Callback for when an entity is destroy. Remove all the components
				belonging to the entity.

			\param entity
				Entity that has been destroyed
			************************************************************************/
			void EntityDestroyed(Entity& entity);

		private:
			// Use unordered map here as const char* is not a C++ string
			// and thus does not overload < operator to be used in STL map
			std::unordered_map<const char*, ComponentType> m_componentTypes;
			std::unordered_map<const char*, IComponentArray*> m_componentArrays;

			ComponentType m_nextComponentType{0};

			template <typename T>
			ComponentArray<T>* GetComponentArray();
		};

#include "ComponentManager.tpp"
	}
}
