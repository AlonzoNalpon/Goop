/*!*********************************************************************
\file   EntityComponentSystem.h
\author w.chinkitbryan\@digipen.edu
\date   9-September-2023
\brief  
  Main coordinator and interface for all the Entity Component System's 
	sub system managers.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once

#include "Component/ComponentManager.h"
#include "Entity/EntityManager.h"
#include "System/SystemManager.h"
#include "../Singleton/Singleton.h"

// Interface for using the different ECS systems
namespace GE
{
	namespace ECS
	{
		class EntityComponentSystem : public Singleton<EntityComponentSystem>
		{
		public:
			/*!*********************************************************************
			\brief
			  Default constructor.
			************************************************************************/
			EntityComponentSystem();

			/*!*********************************************************************
			\brief
			  Default destructor.
			************************************************************************/
			~EntityComponentSystem();

			/*!*********************************************************************
			\brief
			  Calls EntityManager's CreateEntity function.
			  
			\return
			  A new entity
			************************************************************************/
			Entity CreateEntity();

			/*!*********************************************************************
			\brief
			  Calls EntityManager's DestroyEntity function.

			\params entity
			  Entity to destroy.
			************************************************************************/
			void DestroyEntity(Entity& entity);

			/*!*********************************************************************
			\brief
			  Calls ComponentManager's RegisterComponent Function.
			************************************************************************/
			template <typename T>
			void RegisterComponent();

			/*!******************************************************************
			\brief
			  Calls ComponentManager's AddComponent Function.
			********************************************************************/
			template <typename T>
			void AddComponent(Entity& entity, T component);

			/*!******************************************************************
			\brief
			  Calls ComponentManager's RemoveComponent Function.
			********************************************************************/
			template <typename T>
			void RemoveComponent(Entity& entity);

			/*!*********************************************************************
			\brief
			  Calls ComponentManager's GetComponent Function.
			
			\return
				Pointer to entity's component.
				Returns null if component does not exist.
			************************************************************************/
			template <typename T>
			T* GetComponent(const Entity& entity, bool ignoreActive = false);

			/*!*********************************************************************
			\brief
			  Calls ComponentManager's GetComponentSignature Function.
			  
			\return
			  A specific component's signature.
			************************************************************************/
			template <typename T>
			ComponentType GetComponentSignature();

			/*!*********************************************************************
			\brief
			  Calls SystemManager's RegisterSystem function.
			  
			\return
			  A pointer to the system.
			************************************************************************/
			template <typename T>
			T* RegisterSystem();

			/*!*********************************************************************
			\brief
			  Calls SystemManager's RemoveSystem function.
			  
			\return
			  If system was succesfully destroyed.			  
			************************************************************************/
			template <typename T>
			bool RemoveSystem();

			/*!******************************************************************
			\brief 
			    Registers a component to a system.
			********************************************************************/
			template <typename C, typename S>
			void RegisterComponentToSystem();

			/*!******************************************************************
			\brief
					Unregisters a component from a system.
			********************************************************************/
			template <typename C, typename S>
			void UnregisterComponentFromSystem();

			/*!******************************************************************
			\brief
			  Calls SystemManager's SetSignature function.
			
			\param signature
				Signature of all components used in the system.
			********************************************************************/
			template <typename T>
			void SetSystemSignature(const ComponentSignature& signature);

			/*!******************************************************************
			\brief
			  Calls SystemManager's RegisterEntityToSystem function.

			\param entity
				Entity to register.

			\return
				If registered successfully.
			********************************************************************/
			template <typename T>
			bool RegisterEntityToSystem(Entity& entity);

			/*!******************************************************************
			\brief
				Calls SystemManager's UnregisterEntityFromSystem function.

			\param entity
				Entity to unregister.

			\return
				If unregistered successfully.
			********************************************************************/
			template <typename T>
			bool UnregisterEntityFromSystem(Entity& entity);

			/*!*********************************************************************
			\brief
			  Calls SystemManager's UpdateSystems function.
			************************************************************************/
			void UpdateSystems();
		private:
			ComponentManager* m_componentManager;
			EntityManager* m_entityManager;
			SystemManager* m_systemManager;
		};

#include "EntityComponentSystem.tpp"
	}
}
