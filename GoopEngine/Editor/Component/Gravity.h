/*!*********************************************************************
\file   BoxCollider.h
\author c.phua\@digipen.edu
\date   12 September 2023
\brief
	Component for gravity.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

#pragma once
#include <pch.h>

namespace GE::Component
{
	struct Gravity
	{
		using vec2 = GE::Math::dVec2;

		/*!*********************************************************************
		\brief
			Default contructor
		************************************************************************/
		Gravity() : m_gravity{} {}

		/*!*********************************************************************
		\brief
			Overload contructor
		************************************************************************/
		Gravity(vec2 grav) : m_gravity{ grav }
		{
		}

		vec2 m_gravity;
	};
}