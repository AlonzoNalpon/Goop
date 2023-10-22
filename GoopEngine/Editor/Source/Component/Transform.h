/*!*********************************************************************
\file   Transform.h
\author c.phua\@digipen.edu
\date   12 September 2023
\brief
	Component for position, scale and rotation.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Math/GEM.h>

namespace GE
{
	namespace Component
	{
		struct Transform
		{
			using vec3 = Math::dVec3;

			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			Transform() : m_pos{}, m_scale{}, m_rot{} {}

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			Transform(Math::dVec3 const& pos, Math::dVec3 const& scale, double rot)
				: m_pos{ pos }, m_scale{ scale }, m_rot{ rot } {}

			vec3 m_pos;
			vec3 m_scale;
			double m_rot;

			Math::dMat4 m_worldTransform;
		};
	}
}