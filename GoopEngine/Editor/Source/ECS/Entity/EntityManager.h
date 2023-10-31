/*!*********************************************************************
\file   EntityManager.h
\author w.chinkitbryan\@digipen.edu
\date   9-September-2023
\brief  
  Contains the info of the actual entities. And acts as an object pool.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "Entity.h"
#include <vector>
#include <deque>
#include <set>
#include <map>

namespace GE
{
	namespace ECS
	{
		class EntityManager
		{
		public:
			// Max entities in the game
			unsigned int m_maxEntities;
		private:
			// Current number of activiely used EntityIDs
			unsigned int m_entitiesAlive;

			std::vector<ComponentSignature> m_entitySignatures;
			std::vector<Entity> m_entities;
		public:
			/*!*********************************************************************
			\brief
			  Deleted constructor as we should only instantiate a EntityManager with
				a proper number of max entities.
			************************************************************************/
			EntityManager() = delete;
			/*!*********************************************************************
			\brief
			  Overloaded constructor that sets the EntityManager's max entity count.
				Max entities is used for object pooling.

			\param maxEntities
			  Num of max entities available.
			************************************************************************/
			EntityManager(unsigned int maxEntities);

			/*!*********************************************************************
			\brief
			  Default destructor.
			************************************************************************/
			~EntityManager();

			/*!*********************************************************************
			\brief
			  Returns the next avaiable entity from the object pool.
			  
			\return
			  A new entity.
			************************************************************************/
			Entity& CreateEntity();

			/*!*********************************************************************
			\brief
			  Frees an entity and returns it to the object pool.

			\param entity
			  Entity to free
			************************************************************************/
			void DestroyEntity(Entity& entity);

			/*!*********************************************************************
			\brief
			  Returns all currently active entities.
			  
			\return
			  All currently active entities.
			************************************************************************/
			std::vector<Entity>& GetEntities();

			/*!*********************************************************************
			\brief
			  Gets the component signature of the entity. The component signature
				is a bitfield (unsigned int) indicating which components the entity has.

			\param entity
			  Entity's signature to get.

			\return
			  Component signature of the entity
			************************************************************************/
			ComponentSignature GetComponentSignature(const Entity& entity) const;

			/*!*********************************************************************
			\brief
			  Sets the component signature of the entity. The component signature
				is a bitfield (unsigned int) indicatin which component the entity has.

			\param entity
				Entity whose signature is being set.

			\param signature
				Signature to set.
			************************************************************************/
			void SetComponentSignature(Entity& entity, const ComponentSignature& signature);
		};
	}
}
