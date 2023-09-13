#include "Collision.h"

using namespace GE;
using vec2 = GE::Math::dVec2;

//AABB & mouse input
bool Collision::CollSys::Collide(AABB& box, vec2& input)
{
	if (input.x <= box.m_topRight.x && input.x >= box.m_topLeft.x)
	{
		if (input.y <= box.m_topLeft.y && input.y >= box.m_botLeft.y)
		{
			return true;
		}
	}
	return false;
}

//AABB & AABB
bool Collision::CollSys::Collide(AABB& box1, AABB& box2)
{
	
	return false;
}

virtual void Collision::CollSys::Awake()
{

}

virtual void Collision::CollSys::Update()
{

}