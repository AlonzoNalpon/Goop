/*!*********************************************************************
\file   PreRootTransformSystem.h
\author w.chinkitbryan\@digipen.edu
\date   29-October-2023
\brief
	Converts world transfroms	to local transforms.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Math/GEM.h>

namespace GE::Systems
{
	class WorldToLocalTransform
	{
	public:
		/*!*********************************************************************
		\brief
			Propergate transform changes in world to local for all children
			Takes a reference to ECS to prevent multiple Singleton GetInstance
			calls to the ECS

		\param ecs
			Reference to ECS

		\param entity
			Entity that is propergating

		\param parentWorldTrans
			Transform to use to find new local
		************************************************************************/
		static void Propergate(GE::ECS::Entity& entity, GE::Math::dMat4& parentWorldTrans);
	};
}
