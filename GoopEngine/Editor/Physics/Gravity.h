#include <pch.h>

struct Gravity : public GE::ECS::Component
{
	using vec2 = GE::Math::dVec2;

	vec2 m_gravity;
};