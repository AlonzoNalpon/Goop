#pragma once
#include <pch.h>

namespace GE
{
	struct Velocity : public GE::ECS::Component
	{
		using vec2 = GE::Math::dVec2;

		Velocity() : m_acc{}, m_vel{} {}
		Velocity(vec2 acc, vec2 vel) : m_acc{ acc }, m_vel{ vel }
		{
		}

		vec2 m_acc; //starting off as stationary
		vec2 m_vel;
	};
}