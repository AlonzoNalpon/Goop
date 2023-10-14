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
			Entity CreateEntity();

			/*!*********************************************************************
			\brief
			  Calls EntityManager's IsActiveEntity function.

			\param entity
			  Entity to check.

			\return
			  Active flag of an entity.
			************************************************************************/
			bool GetIsActiveEntity(Entity& entity);

			/*!*********************************************************************
			\brief
				Calls EntityManager's SetActiveEntity function.

			\param entity
				Active flag of an entity.

			\param bool
				Active flag of an entity.
			************************************************************************/
			void SetIsActiveEntity(Entity& entity, bool active);

			/*!******************************************************************
			\brief
				Calls EntityManager's GetParentEntity function.
				Returns the entity of the parent of an entity.

			\param[in] entity
				Entity whose parent we are checking.

			\return
				Parent of the entity.
			********************************************************************/
			Entity GetParentEntity(Entity& entity);

			/*!******************************************************************
			\brief
				Calls EntityManager's SetParentEntity function.
				Sets the parent of the an entitiy.

			\param[in] child
				Entity whose parent you are setting.

			\param[in] parent
				Entity which is becoming the parent. Defaults to invalid
				to indicate no parent.
			********************************************************************/
			void SetParentEntity(Entity& child, Entity parent = INVALID_ID);

			/*!******************************************************************
			\brief
				Calls EntityManager's GetChildEntities function.
				Return a vector of all children belonging to an entity.

			\param[in] parent
				Entity whose children you are getting.

			\return
				Vector of entities.
			********************************************************************/
			std::vector<Entity>& GetChildEntities(Entity& parent);

			/*!******************************************************************
			\brief
				Calls EntityManager's AddChildEntities function.
				Adds an entity as a child.

			\param[in] parent
				Entity who you are adding a child to.

			\param[in] child
				Entity who is becoming a child of.
			********************************************************************/
			void AddChildEntity(Entity& parent, Entity& child);

			/*!******************************************************************
			\brief
				Calls EntityManager's RemoveChildEntities function.
				Remove an entity as a child.

			\param[in] parent
				Entity who you are removing from.

			\param[in] child
				Entity who is being removed.
			********************************************************************/
			void RemoveChildEntity(Entity& parent, Entity& child);

			/*!*********************************************************************
			\brief
				Calls EntityManager's DestroyEntity function.

			\params entity
				Entity to destroy.
			************************************************************************/
			void DestroyEntity(Entity& entity);

			/*!******************************************************************
			\brief 
			  Calls EntityManager's GetEntityName function.

			\param[in] entity
				Entity's name to get.

			\return 
				Name of the entity
			********************************************************************/
			std::string GetEntityName(Entity& entity);

			/*!******************************************************************
			\brief 
			  Calls EntityManager's SetEntityName function.

			\param[in] entity
				Entity's name to change.

			\param[in] newName
				Entity's new name.

			\return
				New name of the entity
			********************************************************************/
			std::string SetEntityName(Entity& entity, std::string newName);

			/*!*********************************************************************
			\brief
			  Calls EntityManager's GetEntities function.
			  
			\return
			  Set of entity IDs
			************************************************************************/
			std::set<Entity>& GetEntities();

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
			std::unique_ptr<ComponentManager> m_componentManager;
			std::unique_ptr<EntityManager> m_entityManager;
			std::unique_ptr<SystemManager> m_systemManager;
		};

		/*template <typename T>
		static T GetMonoComponent(Entity entity)
		{
			EntityComponentSystem* ecs = &(EntityComponentSystem::GetInstance());
			return ecs->GetComponent<T>(entity);
		}

		template <typename T>
		static void SetMonoComponent(Entity entity, T newChange)
		{
			EntityComponentSystem* ecs = &(EntityComponentSystem::GetInstance());
			ecs->GetComponent<T>(entity) += newChange;
		}*/


	


#include "EntityComponentSystem.tpp"
	}
}
