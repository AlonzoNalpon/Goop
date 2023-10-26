/*!*********************************************************************
\file   BoxCollider.h
\author c.phua\@digipen.edu
\date   20 September 2023
\brief
	Component for interpolation.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <queue>

namespace GE
{
	namespace Component
	{
		struct Tween
		{
			using vec3 = GE::Math::dVec3;

			std::deque<vec3> m_tweens;
			double m_timePerTween;
			double m_timeTaken;
			double m_timeElapsed;
			vec3 m_originalPos;
			bool m_started;

			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			Tween() {}

			/*!*********************************************************************
			\brief
				Overloaded contructor
			\param time
				Total time for the entire interpolation.
			************************************************************************/
			Tween(double time) : m_timeTaken{ time }, m_timeElapsed{ 0.0 }, m_started{ false } {}

			/*!*********************************************************************
			\brief
				Adding position into queue to calculate interpolation
			\param target
				New position to interpolate to.
			************************************************************************/
			void AddTween(vec3 target)
			{
				m_tweens.push_back(target);
				m_timePerTween = m_timeTaken / m_tweens.size();
			}
		};
	}
}
