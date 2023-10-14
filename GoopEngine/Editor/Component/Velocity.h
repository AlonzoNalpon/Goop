/*!*********************************************************************
\file   Velocity.h
\author c.phua\@digipen.edu
\date   12 September 2023
\brief
	Component for velocity and acceleration.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

#pragma once
#include <pch.h>

namespace GE
{
	namespace Component
	{
		struct Velocity
		{
			using vec2 = GE::Math::dVec2;

			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			Velocity() : m_acc{}, m_vel{}, m_mass{} {}

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			Velocity(vec2 acc, vec2 vel, double mass) : m_acc{ acc }, m_vel{ vel }, m_mass{ mass }
			{
			}

			vec2 m_acc; //starting off as stationary
			vec2 m_vel;
			double m_mass;
		};
	}
}