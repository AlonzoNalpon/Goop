/*!*********************************************************************
\file   Tween.h
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

			struct Action
			{
				vec3 m_target;
				double m_duration;
			};

			std::deque<Action> m_tweens;
			double m_timeTaken;
			double m_timeElapsed;
			vec3 m_originalPos;
			bool m_started;
			bool m_paused;

			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			Tween() : m_timeTaken{ 0.0 }, m_timeElapsed{ 0.0 }, m_started{ false }, m_paused{ false } {}

			/*!*********************************************************************
			\brief
				Adding position into queue to calculate interpolation
			\param target
				New position to interpolate to.
			************************************************************************/
			void AddTween(vec3 target, double duration)
			{
				m_tweens.emplace_back(target, duration);
			}
		};
	}
}
