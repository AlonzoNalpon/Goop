#pragma once
#include <pch.h>

namespace GE
{
	struct Transform : public GE::ECS::Component
	{
		using vec2 = Math::dVec2;
		Transform() : m_pos{}, m_scale{}, m_rot{} {}
		Transform(Math::dVec2 const& pos, Math::dVec2 const& scale, double rot)
			: m_pos{pos}, m_scale{scale}, m_rot{rot} {}

		Math::dVec2 m_pos;
		Math::dVec2 m_scale;
		double m_rot;
	};
}