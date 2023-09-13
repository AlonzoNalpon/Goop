/*!*********************************************************************
\file   SystemManager.h
\author w.chinkitbryan\@digipen.edu
\date   9-September-2023
\brief  
  Container and manager for all existing entity systems.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <unordered_map>
#include <queue>
#include "../Entity/Entity.h"
#include "System.h"

namespace GE
{
	namespace ECS
	{
		class SystemManager
		{
		public:
			/*!*********************************************************************
			\brief
			  Defaulted constructor.			  
			************************************************************************/
			SystemManager() = default;
			/*!*********************************************************************
			\brief
			  Properly frees all remaining systems from memory.		
			************************************************************************/
			~SystemManager();

			/*!*********************************************************************
			\brief
			  Registers a system to the manager.
			  
			\return
			  Pointer to the registered system.
			************************************************************************/
			template <typename T>
			T* RegisterSystem();
			/*!*********************************************************************
			\brief
			  Removes and destroys a system in the manager.
			  
			\return
			  If system was succesfully destroyed.
			************************************************************************/
			template <typename T>
			bool RemoveSystem();
			/*!******************************************************************
			\brief
			  Sets the component signature for the system. The component
				signature is a bitfield (unsigned int) that shows what components
				the system requires in an entity.

			\param signature
				Signature of all components used in the system.
			********************************************************************/
			template <typename T>
			bool SetSignature(const ComponentSignature& signature);
			/*!*********************************************************************
			\brief
			  Callback for when entity is destroyed. Unregisters an entity from the
				system.

			\param entity
			  Entity that was destroyed.
			************************************************************************/
			void EntityDestroyed(const Entity& entity);
			/*!*********************************************************************
			\brief
			  Updates the list of entities by checking if the new signature of the
				input entity is still valid to be used in the system.

			\param entity
				Entity whose signature changed.

			\param signature
			  New signature.
			************************************************************************/
			void EntitySignatureChanged(Entity& entity, const ComponentSignature& signature);

			/*!******************************************************************
			\brief
			  Registers an entity to a system. This adds the entity to the 
				system's entity list.

			\param
				Entity to register.

			\return
				Returns if it was registered successfully.
			********************************************************************/
			template <typename T>
			bool RegisterEntityToSystem(Entity& entity);
			/*!******************************************************************
			\brief
				Unregisters an entity from a system. This removes the entity from
				the system's entity list.

			\param
				Entity to unregister.

			\return
				Returns if it was unregistered successfully.
			********************************************************************/
			template <typename T>
			bool UnregisterEntityFromSystem(Entity& entity);

			/*!*********************************************************************
			\brief
			  Calls the update function for all systems in the manager.
			************************************************************************/
			void UpdateSystems();
			/*!*********************************************************************
			\brief
			  Called by the framerate controller at a fixed time intervals. Usually
				reserved for physics calculations.
			************************************************************************/
			void UpdateSystemsFixed();
		private:
			// This is a map of unique signatures each system has
			// a system signature consist of the signatures of all the
			// components used in the system
			std::unordered_map<const char*, ComponentSignature> m_signatures;
			std::unordered_map<const char*, System*> m_systems;
			std::queue<const char*> m_uninitializedSystems;
		};

#include "SystemManager.tpp"
	}
}
