#pragma once
#include <pch.h>

namespace GE::Component
{
	struct Gravity
	{
		using vec2 = GE::Math::dVec2;

		Gravity() : m_gravity{} {}
		Gravity(vec2 grav) : m_gravity{ grav }
		{
		}

		vec2 m_gravity;
	};
}