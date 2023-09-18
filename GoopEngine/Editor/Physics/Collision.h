#pragma once
#include <pch.h>
#include "BoxCollider.h"

using namespace GE::Math;

namespace GE
{
	namespace Collision
	{
		class CollisionSystem : public GE::ECS::System
		{
		public:
			//collision checks:
			//AABB & mouse input
			bool Collide(AABB& box, dVec2& input);

			//AABB & AABB
			bool Collide(AABB& box1, AABB& box2);

			void Awake();

			void Update();

		private:
			GE::ECS::EntityComponentSystem* m_ecs;
			void UpdateAABB(AABB& entity, const dVec2& newCenter);
		};
	}
}