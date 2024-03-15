/*!*********************************************************************
\file   PhysicsSystem.h
\author c.phua\@digipen.edu
\date   12 September 2023
\brief
	Physics system.
	Updates entity's velocity with respect to delta time.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

#pragma once
#include <ECS/System/System.h>

namespace GE
{
	namespace Systems
	{
		class PhysicsSystem : public GE::ECS::System
		{
		public:
			/*!*********************************************************************
			\brief
				Updates velocity with respect to delta time.
			************************************************************************/
			void FixedUpdate();
		};
	}
}