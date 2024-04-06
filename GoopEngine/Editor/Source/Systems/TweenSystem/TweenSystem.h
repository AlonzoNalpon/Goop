/*!*********************************************************************
\file   TweenSystem.h
\author c.phua\@digipen.edu
\co-authors a.nalpon\@digipen.edu
\date   20-September-2023
\brief
	Uses tweening to update entity's position.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

#pragma once
#include <Math/GEM.h>
#include <ECS/System/System.h>

using vec3 = GE::Math::dVec3;
namespace GE::Systems
{
	using namespace Graphics;
	class TweenSystem : public GE::ECS::System
	{
	public:
		/*!*********************************************************************
		\brief
			System update.
		************************************************************************/
		void Update();

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

		/*!*********************************************************************
		\brief
		  Computes the interpolated color based on a start time, end time and
			the current time
		\param start
			The starting color
		\param end
			The end ccolor
		\param normalisedTime
			The normalized time out of 1.f
		\return
		  The interpolated color
		************************************************************************/
		Colorf Tweening(Colorf start, Colorf end, double normalisedTime);
	};
}