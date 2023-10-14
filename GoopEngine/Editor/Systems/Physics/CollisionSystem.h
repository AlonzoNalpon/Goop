/*!*********************************************************************
\file   CollisionSystem.h
\author c.phua\@digipen.edu
\date   16 September 2023
\brief
	Collision system.
	Checks for AABB-to-AABB collision.
	Checks for AABB-to-mouse collision.
	Updates AABB collision box of entity according to it's new center.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

#pragma once
#include <pch.h>
#include <Component/BoxCollider.h>
#include <Component/Transform.h>

using namespace GE::Component;

namespace GE
{
	namespace Systems
	{
		class CollisionSystem : public GE::ECS::System
		{
		public:
			/*!*********************************************************************
			\brief
				Collision check fro AABB-to-mouse.
			\param box
				AABB collider for entity
			\param input
				Mouse input
			\return bool
				True if collided.
				False if not collided.
			************************************************************************/
			bool Collide(BoxCollider& box, Math::dVec2& input);

			/*!*********************************************************************
			\brief
				Collision check fro AABB-to-AABB.
			\param box1
				AABB collider for first entity
			\param box2
				AABB collider for second entity
			\return bool
				True if collided.
				False if not collided.
			************************************************************************/
			bool Collide(BoxCollider& box1, BoxCollider& box2);

			/*!*********************************************************************
			\brief
				Updates collision boxes for all existing entities.
				Updates collision flags for all existing entities.
			************************************************************************/
			void Update();

		private:
			/*!*********************************************************************
			\brief
				Updates collision box for entity according to it's new center.
			\param entity
				Entity to update.
			\param newCenter
				Entity's new center position.
			\param scale
				Entity's size.
			************************************************************************/
			void UpdateAABB(BoxCollider& entity, const Math::dVec2& newCenter, Transform& scale);
		};
	}
}