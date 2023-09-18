#include <pch.h>

struct Transform : public GE::ECS::Component
{
	using vec2 = GE::Math::dVec2;

	vec2 m_pos;
	vec2 m_scale;
	double m_rot;
};