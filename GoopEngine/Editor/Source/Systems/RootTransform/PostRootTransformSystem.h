/*!*********************************************************************
\file   PostRootTransformSystem.h
\author w.chinkitbryan\@digipen.edu
\date   29-October-2023
\brief  
  2nd pass of computing transforms. This pass converts local transfroms
	to world transforms.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Math/GEM.h>
#include <ECS/System/System.h>

namespace GE::Systems
{
	class PostRootTransformSystem : public GE::ECS::System
	{
		// Intentionally override this function as
		// it has different behaviour as it will handle
		// inactive entities as well
		void Update();

	public:
		static void Propergate(GE::ECS::EntityComponentSystem& ecs, GE::ECS::Entity& entity, const Math::dMat4& parentWorldTrans);
	};
}
