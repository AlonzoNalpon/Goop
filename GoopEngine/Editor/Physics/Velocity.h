#include <pch.h>

struct Velocity : public GE::ECS::Component
{
	using vec2 = GE::Math::dVec2;

	vec2 m_acc = 0; //starting off as stationary
	vec2 m_vel = 0;
};