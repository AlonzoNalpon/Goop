#pragma once
//#include <pch.h>
#include "../ECS/Component/Component.h"
#include "../Math/GEM.h"

struct Velocity : public GE::ECS::Component
{
	using vec2 = GE::Math::dVec2;

	vec2 m_acc; //starting off as stationary
	vec2 m_vel;
};