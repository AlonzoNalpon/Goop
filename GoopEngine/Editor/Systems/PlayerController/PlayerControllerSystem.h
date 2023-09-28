/*!*********************************************************************
\file   BoxCollider.h
\author c.phua\@digipen.edu
\date   20 September 2023
\brief
	Player controller system.
	Uses tweening to update entity's position.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

#pragma once
#include <pch.h>
#include <Component/Transform.h>

using vec2 = GE::Math::dVec2;

namespace GE
{
	namespace Systems
	{
		class PlayerControllerSystem : public GE::ECS::System, public GE::Events::EventListener
		{
		public:
			/*!*********************************************************************
			\brief
				System awake.
			************************************************************************/
			void Awake();

			/*!*********************************************************************
			\brief
				System startup.
			************************************************************************/
			void Start();

			/*!*********************************************************************
			\brief
				System update.
			************************************************************************/
			void Update();

			/*!*********************************************************************
			\brief
				Event handler for the system.
			\param event
				Event occuring.
			************************************************************************/
			void HandleEvent(Events::Event const* event) override;

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
			vec2 Tweening(vec2 start, vec2 end, double normalisedTime);
		};

	}
}




