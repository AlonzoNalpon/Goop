#pragma once
#include <pch.h>

namespace GE
{
	namespace Component
	{
		struct LinearForce
		{
			using vec2 = GE::Math::dVec2;

			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			LinearForce() : m_direction{}, m_magnitude{}, m_lifetime{}, m_age{}, m_isActive{} {}

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			LinearForce(vec2 dir, vec2 mag, double lifetime, double age = 0, bool active = false) : m_direction{dir}, m_magnitude{mag}, m_lifetime{lifetime},
			m_age{age}, m_isActive{active}
			{
			}

			vec2 m_direction;
			vec2 m_magnitude;
			double m_lifetime;
			double m_age;
			bool m_isActive;
		};

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
}