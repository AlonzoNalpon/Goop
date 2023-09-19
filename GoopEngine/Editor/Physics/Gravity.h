#pragma once
//#include <pch.h>
//#include "../ECS/Component/Component.h"
#include "../Math/GEM.h"

namespace GE
{
	struct Gravity //: public GE::ECS::Component
	{
		using vec2 = GE::Math::dVec2;

		Gravity() : m_gravity{} {}
		Gravity(vec2 grav) : m_gravity{ grav }
		{
		}

		vec2 m_gravity;
	};
}