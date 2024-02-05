/*!*********************************************************************
\file   System.h
\author w.chinkitbryan\@digipen.edu
\date   9-September-2023
\brief  
  System base class. This should be inherited by all systems, where the
	functions will be called by the ECS.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <set>
#include "../Entity/Entity.h"

namespace GE
{
	namespace ECS
	{
		// Forward declare
		class EntityComponentSystem;
		class System
		{
		public:
			/*!*********************************************************************
			\brief
			  Defaulted constructor
			************************************************************************/
			System() = default;
			/*!*********************************************************************
			\brief
			  Defaulted destructor
			************************************************************************/
			virtual ~System() = default;

			/*!*********************************************************************
			\brief
			  This is a sudo constructor for the system and is called on creation
				of the system
			************************************************************************/
			virtual void Awake();

			/*!*********************************************************************
			\brief
			  This is the initialize function of the system and is called before the
				first update loop is raa.
			************************************************************************/
			virtual void Start();

			/*!*********************************************************************
			\brief
			  Update function is called by the ECS and should process all entities.
			************************************************************************/
			virtual void Update();

			/*!*********************************************************************
			\brief
			  Late update function is called by the ECS after update. This function
				is to allow for delayed processing where required.
			************************************************************************/
			virtual void LateUpdate();

			/*!*********************************************************************
			\brief
			  This update should be called by the main game loop at fixed time
				intervals.
			************************************************************************/
			virtual void FixedUpdate();

			/*!*********************************************************************
			\brief
			  This is a sudo destructor.
			************************************************************************/
			virtual void OnDestroyed();

			/*!******************************************************************
			\brief 
			  Gets a set of entities that this system should update
			\return 
				Set containing entities to update
			********************************************************************/
			std::set<Entity>& GetUpdatableEntities();

			/*!*********************************************************************
			\brief
			  Returns the set of entities registered to current system.

			\return 
				Set containting all active entities.
			************************************************************************/
			std::set<Entity>& GetEntities();

			/*!*********************************************************************
			\brief
				Returns the set of inactive registered to current system.

			\return 
				Set containting all inactive entities.
			************************************************************************/
			std::set<Entity>& GetInActiveEntities();

			/*!******************************************************************
			\brief 
			  Returns a set of all entities active or inactive.

			\return 
				Set containting all entities.
			********************************************************************/
			std::set<Entity>& GetAllEntities();

			/*!******************************************************************
			\brief
				Updates all entities in the system to remove them from active
				entity list

			\param[in] entity
				Entity to update

			\param[in] newState
				New active state
			********************************************************************/
			void EntityActiveStateChanged(Entity const& entity, bool newState);

		protected:
			std::set<Entity> m_entities;
			std::set<Entity> m_inactiveEntities;
			std::set<Entity> m_allEntities;
			EntityComponentSystem* m_ecs;
		private:
			std::set<Entity> m_updatables;
		};
	}
}
