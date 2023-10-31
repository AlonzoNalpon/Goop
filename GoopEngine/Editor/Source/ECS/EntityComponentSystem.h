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
#include <memory>

// Interface for using the different ECS systems
namespace GE
{
	namespace ECS
	{
		class EntityComponentSystem : public Singleton<EntityComponentSystem>
		{
		public:
			friend class Entity;

			/*!*********************************************************************
			\brief
				Default constructor.
			************************************************************************/
			EntityComponentSystem();

			/*!*********************************************************************
			\brief
				Calls EntityManager's CreateEntity function.

			\return
				A new entity
			************************************************************************/
			Entity& CreateEntity();

			/*!*********************************************************************
			\brief
				Calls EntityManager's DestroyEntity function.

			\params entity
				Entity to destroy.
			************************************************************************/
			void DestroyEntity(Entity& entity);

			/*!*********************************************************************
			\brief
				Calls EntityManager's GetEntities function.

			\return
				Set of entity IDs
			************************************************************************/
			std::vector<Entity>& GetEntities();

			/*!*********************************************************************
			\brief
				Calls ComponentManager's RegisterComponent function.
			************************************************************************/
			template <typename T>
			void RegisterComponent();

			/*!******************************************************************
			\brief
				Calls ComponentManager's AddComponent function.
			********************************************************************/
			template <typename T>
			void AddComponent(Entity& entity, T component);

			/*!******************************************************************
			\brief
				Calls ComponentManager's RemoveComponent function.
			********************************************************************/
			template <typename T>
			void RemoveComponent(Entity& entity);

			/*!*********************************************************************
			\brief
				Calls ComponentManager's GetComponent function.

			\param entity
				Entity from which the component is gotten from.

			\return
				Pointer to entity's component.
				Returns null if component does not exist or is inactive.
			************************************************************************/
			template <typename T>
			T* GetComponent(const Entity& entity);

			/*!*********************************************************************
			\brief
				Calls ComponentManager's HasComponent function.

			\param entity
				Entity from which to check if component exist

			\return
				Has component
			************************************************************************/
			template <typename T>
			bool HasComponent(const Entity& entity);

			/*!*********************************************************************
			\brief
				Calls ComponentManager's GetComponentSignature function.

			\return
				A specific component's signature.
			************************************************************************/
			template <typename T>
			ComponentType GetComponentSignature();

			/*!******************************************************************
			\brief
				Calls EntityManager's GetComponentSignature function.

			\return
				The component signature makeup of an entity
			********************************************************************/
			ComponentSignature GetComponentSignature(Entity& entity);

			/*!******************************************************************
			\brief
				Calls SystemManager's GetSystem function.

			\return
				Pointer to a system of type T
			********************************************************************/
			template <typename T>
			T* GetSystem();

			/*!******************************************************************
			\brief
				Returns a map of system signatures, with keys being the typename
				of a system

			\return
				Map of systems
			********************************************************************/
			std::unordered_map<const char*, ComponentSignature>& GetSystemSignatures();

			/*!******************************************************************
			\brief
				Returns a sorted map of systems values being the typename of a
				system. Keys are ambigious and should not be used to iterate.
				Only iterate with a ranged for as intergers maybe not be
				in sequence.

			\return
				Map of systems names
			********************************************************************/
			std::map<int, const char*>& GetSystemIndexes();

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
				Call's SystemManager's GetSignature function.

			\return
				Component signature of system
			********************************************************************/
			template <typename T>
			ComponentSignature GetSystemSignature();

			/*!******************************************************************
			\brief
				This function is depreciated. Behaviour may be undefined.
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
				This function is depreciated. Behaviour may be undefined.
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

			/*!*********************************************************************
			\brief
				Update systems except the following systems. Usage of system name is
				typeid(System Class).name().

				User has to append a nullptr to indicate the end of the parameter list

			\param systemCount
				Number of systems

			\param ...
				typeid(System Class).name() of systems
			************************************************************************/
#ifndef NO_IMGUI
			void UpdateSystems(int systemCount, ...);
#endif // !NO_IMGUI

		private:
			std::unique_ptr<ComponentManager> m_componentManager;
			std::unique_ptr<EntityManager> m_entityManager;
			std::unique_ptr<SystemManager> m_systemManager;
		};

#include "EntityComponentSystem.tpp"
	}
}
