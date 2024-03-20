/*!*********************************************************************
\file   ScriptSystem.h
\author c.phua\@digipen.edu
\date   20 September 2023
\brief
	Updates all updatable script on the entities

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

#pragma once
#include <Math/GEM.h>
#include <ECS/System/System.h>
#include <Component/Transform.h>

using vec3 = GE::Math::dVec3;

namespace GE
{
	namespace Systems
	{
		class ScriptSystem : public GE::ECS::System
		{
		public:
			/*!*********************************************************************
			\brief
				System update.
			************************************************************************/
			void Update();
		};

	}
}




