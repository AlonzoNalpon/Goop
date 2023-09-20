#pragma once
#include <pch.h>
#include <Component/BoxCollider.h>

using namespace GE::Component;

namespace GE
{
	namespace Systems
	{
		class CollisionSystem : public GE::ECS::System
		{
		public:
			//collision checks:
			//AABB & mouse input
			bool Collide(AABB& box, Math::dVec2& input);

			//AABB & AABB
			bool Collide(AABB& box1, AABB& box2);

			void Update();

		private:
			void UpdateAABB(AABB& entity, const Math::dVec2& newCenter);
		};
	}
}