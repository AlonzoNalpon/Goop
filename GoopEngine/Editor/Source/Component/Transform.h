/*!*********************************************************************
\file   Transform.h
\author c.phua\@digipen.edu
\date   12-September-2023
\brief
	Component for position, scale and rotation.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
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
			Transform() : m_pos{ 0.0, 0.0, 0.0 }, m_scale{ 1.0, 1.0, 1.0 }, m_rot{ 0.0, 0.0, 0.0 },
				m_worldPos{ 0.0, 0.0, 0.0 }, m_worldScale{ 1.0, 1.0, 1.0 }, m_worldRot{ 0.0, 0.0, 0.0 } {}

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			Transform(Math::dVec3 const& pos, Math::dVec3 const& scale, Math::dVec3 const& rot)
				: m_worldPos{ pos }, m_worldScale{ scale }, m_worldRot{ rot } {}

			vec3 m_pos;
			vec3 m_scale;
			vec3 m_rot;

			vec3 m_worldPos;
			vec3 m_worldScale;
			vec3 m_worldRot;

			Math::dMat4 m_parentWorldTransform;
			Math::dMat4 m_worldTransform;
			Math::dMat4 m_renderTransform;
		};
	}
}