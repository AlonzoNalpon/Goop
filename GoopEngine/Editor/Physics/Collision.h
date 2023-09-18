#include "../ECS/System/System.h"
#include "../Math/GEM.h"

using vec2 = GE::Math::dVec2;

namespace GE
{
	//collision box -> AABB
	struct AABB
	{
		vec2 m_topLeft;
		vec2 m_topRight;
		vec2 m_botRight;
		vec2 m_botLeft;
		double m_width;
		double m_height;
		vec2 m_min;
		vec2 m_max;
		vec2 m_center;
		//can use AABB & position(center)
	};

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

		private:
		};
	}
}