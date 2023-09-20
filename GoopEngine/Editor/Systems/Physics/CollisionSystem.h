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
			//collision checks:
			//AABB & mouse input
			bool Collide(BoxCollider& box, Math::dVec2& input);

			//AABB & AABB
			bool Collide(BoxCollider& box1, BoxCollider& box2);

			void Update();

		private:
			void UpdateAABB(BoxCollider& entity, const Math::dVec2& newCenter, Transform& scale);
		};
	}
}