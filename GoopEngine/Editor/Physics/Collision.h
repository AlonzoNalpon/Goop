#include <pch.h>
#include "BoxCollider.h"

namespace GE
{
	namespace Collision
	{
		class CollSys : public GE::ECS::System
		{
		public:
			//collision checks:
			//AABB & mouse input
			bool Collide(AABB& box, vec2& input);

			//AABB & AABB
			bool Collide(AABB& box1, AABB& box2);

			void Awake();

			void Update();

		private:
			EntityComponentSystem* m_ecs;
			void UpdateAABB(AABB& entity, const vec2& newCenter);
		};
	}
}