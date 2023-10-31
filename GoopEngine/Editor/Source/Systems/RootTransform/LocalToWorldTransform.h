/*!*********************************************************************
\file   LocalToWorldTransform.h
\author w.chinkitbryan\@digipen.edu
\date   29-October-2023
\brief  
  Converts local transfroms	to world transforms.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Math/GEM.h>

namespace GE::Systems
{
	class LocalToWorldTransform
	{
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
		static void Propergate(GE::ECS::Entity& entity, const Math::dMat4& parentWorldTrans);
	};
}
