/*!*********************************************************************
\file   Transform.h
\author c.phua\@digipen.edu
\date   12 September 2023
\brief
	Component for position, scale and rotation.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once

namespace GE
{
	namespace Component
	{
		struct Transform
		{
			using vec2 = Math::dVec2;

			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			Transform() : m_pos{}, m_scale{}, m_rot{} {}

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			Transform(Math::dVec2 const& pos, Math::dVec2 const& scale, double rot)
				: m_pos{ pos }, m_scale{ scale }, m_rot{ rot } {}

			vec2 m_pos;
			vec2 m_scale;
			double m_rot;

			Math::dMat3 m_worldTransform;
		};
	}
}