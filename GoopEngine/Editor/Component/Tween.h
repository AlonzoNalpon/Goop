#pragma once
#include <pch.h>

namespace GE
{
	namespace Component
	{
		struct Tween : public GE::ECS::Component
		{
			using vec2 = GE::Math::dVec2;

			std::queue<vec2> m_tweens;
			double m_timePerTween;
			double m_timeTaken;
			double m_timeElapsed;
			vec2 m_originalPos;
			bool m_started;

			Tween(double time) : m_timeTaken{ time }, m_timeElapsed{ 0.0 }, m_started{ false } {}

			void AddTween(vec2 target)
			{
				m_tweens.emplace(target);
				m_timePerTween = m_timeTaken / m_tweens.size();
			}
		};
	}
}
