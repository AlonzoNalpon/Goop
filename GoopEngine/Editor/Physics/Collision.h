#pragma once
#include <pch.h>
#include "BoxCollider.h"

namespace GE
{
	namespace Collision
	{
		class CollisionSystem : public GE::ECS::System
		{
		public:
			//collision checks:
			//AABB & mouse input
			bool Collide(AABB& box, Math::dVec2& input);

			//AABB & AABB
			bool Collide(AABB& box1, AABB& box2);

			void Awake();

			void Update();

		private:
			GE::ECS::EntityComponentSystem* m_ecs;
			void UpdateAABB(AABB& entity, const Math::dVec2& newCenter);
		};
	}
}