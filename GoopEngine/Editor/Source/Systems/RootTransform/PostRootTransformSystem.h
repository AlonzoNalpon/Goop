/*!*********************************************************************
\file   PostRootTransformSystem.h
\author w.chinkitbryan\@digipen.edu
\date   29-October-2023
\brief  
  2nd pass of computing transforms. This pass converts local transfroms
	to world transforms.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Math/GEM.h>
#include <ECS/System/System.h>

namespace GE::Systems
{
	class PostRootTransformSystem : public GE::ECS::System
	{
		/*!*********************************************************************
		\brief
			This should be called right before rendering system
		************************************************************************/
		void LateUpdate();

	public:
		/*!*********************************************************************
		\brief
			Propergate all transform changes to children
			Takes a reference to ECS to prevent multiple Singleton GetInstance
			calls to the ECS

		\param ecs
			Reference to ECS

		\param entity
			Entity that is propergating
		************************************************************************/
		static void Propergate(GE::ECS::Entity const& entity, const Math::dMat4& parentWorldTrans);
	};
}
