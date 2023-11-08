/*!*********************************************************************
\file   TweenSystem.h
\author c.phua\@digipen.edu
\date   20 September 2023
\brief
	Uses tweening to update entity's position.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

#pragma once
#include <Math/GEM.h>
#include <ECS/System/System.h>

using vec3 = GE::Math::dVec3;

namespace GE::Systems
{
	class TweenSystem : public GE::ECS::System
	{
	public:
		/*!*********************************************************************
		\brief
			System update.
		************************************************************************/
		void FixedUpdate();

		/*!*********************************************************************
		\brief
			Interpolates between two points with a normalised time value.
		\param start
			Start point.
		\param end
			End point.
		\param normalisedTime
			Normalised time.
		************************************************************************/
		vec3 Tweening(vec3 start, vec3 end, double normalisedTime);
	};
}