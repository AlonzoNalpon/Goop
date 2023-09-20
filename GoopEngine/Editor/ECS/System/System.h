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

			/*!*********************************************************************
			\brief
			  Returns the list of entities registered to current system.
			************************************************************************/
			virtual std::set<Entity>& GetEntities();

		protected:
			std::set<Entity> m_entities;
			EntityComponentSystem* m_ecs;

		};
	}
}
