/*!*********************************************************************
\file   PreRootTransformSystem.h
\author w.chinkitbryan\@digipen.edu
\date   29-October-2023
\brief
	1st pass of computing transforms. This pass converts world tramsfroms
	to local transforms.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Math/GEM.h>
#include <ECS/System/System.h>

namespace GE::Systems
{
	class PreRootTransformSystem : public GE::ECS::System
	{
		/*!*********************************************************************
		\brief
			This should be call before almost any system
		************************************************************************/
		void Update();

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
		static void Propergate(GE::ECS::EntityComponentSystem& ecs, GE::ECS::Entity& entity);
	};
}
