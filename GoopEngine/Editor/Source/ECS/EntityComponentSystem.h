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
	// forward declaration
	namespace ObjectFactory { class ObjectFactory; }

	namespace ECS
	{
		// for access to exclusive CreateEntity() function
		class Exclusive { friend class ObjectFactory::ObjectFactory; Exclusive(){} };

		class EntityComponentSystem : public Singleton<EntityComponentSystem>
		{
		private:
			std::unique_ptr<ComponentManager> m_componentManager;
			std::unique_ptr<EntityManager> m_entityManager;
			std::unique_ptr<SystemManager> m_systemManager;

		public:
			/*!*********************************************************************
			\brief
				Default constructor.
			************************************************************************/
			EntityComponentSystem();

			/*!*********************************************************************
			\brief
			  Return the max number of entities allowed in the system.
			  
			\return
			  Max entities
			************************************************************************/
			unsigned int GetMaxEntities();

			/*!*********************************************************************
			\brief
				Calls EntityManager's CreateEntity function.

			\return
				A new entity
			************************************************************************/
			Entity CreateEntity();

			/*!*********************************************************************
			\brief
				Calls EntityManager's CreateEntity overload. This functions allows
				the user to create an entity with a given ID and name, which is
				not assigned by the ECS.

				To prevent misuse, this function can only be invoked by classes 
				that the "Exclusive" class explicitly friends. Only friend classes
				can create an instance of Exclusive by specifying "{}" in the arg list.

			\param entity
				Entity ID which you will be using

			\param name
				Name of the entity being created
			************************************************************************/
			void CreateEntity(Exclusive key, Entity entity, std::string name = "");

			/*!*********************************************************************
			\brief
				Calls EntityManager's IsActiveEntity function.

			\param entity
				Entity to check.

			\return
				Active flag of an entity.
			************************************************************************/
			bool GetIsActiveEntity(Entity const& entity) const;

			/*!*********************************************************************
			\brief
				Calls EntityManager's SetActiveEntity function.

			\param entity
				Active flag of an entity.

			\param bool
				Active flag of an entity.
			************************************************************************/
			void SetIsActiveEntity(Entity const& entity, bool active);

			/*!******************************************************************
			\brief
				Calls EntityManager's GetParentEntity function.
				Returns the entity of the parent of an entity.

			\param[in] entity
				Entity whose parent we are checking.

			\return
				Parent of the entity.
			********************************************************************/
			Entity GetParentEntity(Entity const& entity) const;

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
			void SetParentEntity(Entity const& child, Entity parent = INVALID_ID);

			/*!******************************************************************
			\brief
				Calls EntityManager's GetChildEntities function.
				Return a vector of all children belonging to an entity.

			\param[in] parent
				Entity whose children you are getting.

			\return
				Vector of entities.
			********************************************************************/
			std::set<Entity>& GetChildEntities(Entity const& parent);

			/*!******************************************************************
			\brief
				Calls EntityManager's AddChildEntities function.
				Adds an entity as a child.

			\param[in] parent
				Entity who you are adding a child to.

			\param[in] child
				Entity who is becoming a child of.
			********************************************************************/
			void AddChildEntity(Entity const& parent, Entity const& child);

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
			std::string GetEntityName(Entity const& entity) const;

			/*!*********************************************************************
			\brief
			  Gets an entity by name. (Will loop through every entity to find
				matching name)
			\params
			  name
			\return
			  Entity object. Invalid object ID if none can be found
			************************************************************************/
			Entity			GetEntity(std::string const& name);

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
			std::string SetEntityName(Entity const& entity, std::string newName);

			/*!*********************************************************************
			\brief
			  Calls EntityManager's GetEntityFromName function.

			\param entityName
			  Entity in question

			\return
			  Entity ID
			************************************************************************/
			Entity GetEntityFromName(std::string entityName);

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
				Calls ComponentManager's HasComponent function.

			\param entity
				Entity from which to check if component exist

			\return
				Has component
			************************************************************************/
			template <typename T>
			bool HasComponent(const Entity& entity) const;

			/*!*********************************************************************
			\brief
				Calls ComponentManager's GetComponentSignature function.

			\return
				A specific component's signature.
			************************************************************************/
			template <typename T>
			ComponentType GetComponentSignature() const;

			/*!******************************************************************
			\brief
				Calls EntityManager's GetComponentSignature function.

			\param entity
				Entity to get the signature of

			\return
				The component signature makeup of an entity
			********************************************************************/
			ComponentSignature GetComponentSignature(Entity& entity) const;

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
#ifndef IMGUI_DISABLE
			void UpdateSystems(int systemCount, ...);
#endif // !IMGUI_DISABLE
		};

#include "EntityComponentSystem.tpp"
	}
}
