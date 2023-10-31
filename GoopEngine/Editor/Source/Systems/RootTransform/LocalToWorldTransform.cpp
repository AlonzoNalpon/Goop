/*!*********************************************************************
\file   PostRootTransformSystem.cpp
\author w.chinkitbryan\@digipen.edu
\date   29-October-2023
\brief
  Converts local transfroms to world transforms.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "LocalToWorldTransform.h"
#include <Component/Transform.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "TransformSystemHelper.h"
#include <Graphics/GraphicsEngine.h>
#include <Component/Sprite.h>

void GE::Systems::LocalToWorldTransform::Propergate(GE::ECS::Entity& entity, const Math::dMat4& parentWorldTrans)
{
	static GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();

	entity.m_parentWorldTransform = parentWorldTrans;

	/////////////////////////////////////
	// Setting world transformation data
	/////////////////////////////////////
	// If has parent, set my world transform to using parent
	// no parent means local is world transform
	if (entity.GetParent() != GE::ECS::INVALID_ID)
	{
		entity.m_worldPos = parentWorldTrans * GE::Math::dVec4(entity.m_pos, 1);
		entity.m_worldScale = entity.m_scale.GetRaw() * entity.GetParent().m_scale.GetRaw();
		entity.m_worldRot = entity.m_rot.GetRaw() + entity.GetParent().m_rot.GetRaw();
	}
	else
	{
		entity.m_worldPos = entity.m_pos;
		entity.m_worldScale = entity.m_scale;
		entity.m_worldRot = entity.m_rot;
	}

	Math::dMat4 T
	{
		{ 1, 0, 0, entity.m_worldPos.GetRaw().x },
		{ 0, 1, 0, entity.m_worldPos.GetRaw().y },
		{ 0, 0, 1, entity.m_worldPos.GetRaw().z },
		{ 0, 0, 0, 1 }
	};
	Math::dMat4 S
	{
		{ entity.m_worldScale.GetRaw().x, 0, 0, 0 },
		{ 0, entity.m_worldScale.GetRaw().y, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	double rad = (entity.m_worldRot.GetRaw().z / 180.0) * M_PI;
	Math::dMat4 Z
	{
		{ std::cos(rad), -std::sin(rad), 0, 0 },
		{ std::sin(rad), std::cos(rad), 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	rad = (entity.m_worldRot.GetRaw().y / 180.0) * M_PI;
	Math::dMat4 Y
	{
		{ std::cos(rad), std::sin(rad), 0, 0 },
		{ 0, 1, 0, 0 },
		{ -std::sin(rad), 0, std::cos(rad), 0 },
		{ 0, 0, 0, 1 }
	};
	rad = (entity.m_worldRot.GetRaw().x / 180.0) * M_PI;
	Math::dMat4 X
	{
		{ 1, 0, 0, 0 },
		{ 0, std::cos(rad), -std::sin(rad), 0 },
		{ 0, std::sin(rad), std::cos(rad), 0 },
		{ 0, 0, 0, 1 }
	};

	double scaleX{1.0}, scaleY{1.0}; // default: 1

	// Precompute every matrix that is the same
	Math::dMat4 resultMtx = T * X * Y * Z;
	// Compute render matrix to use sprite dimensions (This matrix used for render only)
	if (ecs.HasComponent<Component::Sprite>(entity))
	{
		Component::Sprite* sprite = ecs.GetComponent<Component::Sprite>(entity); // get sprite component
		// Now we set the sprite information for dimensions for scaling
		scaleX = sprite->spriteData.info.width;			
		scaleY = sprite->spriteData.info.height;

		// Texture scale to follow dimensions of sprite
		Math::dMat4 textureScale
		{
			{ scaleX, 0, 0, 0 },
			{ 0, scaleY, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 }
		};

		entity.m_renderTransform = resultMtx * textureScale * S;
	}

	entity.m_worldTransform = resultMtx * S;

	// End condition no children
	if (entity.GetChildren().empty())
	{
		return;
	}

	// Update all children based ownself's new world transform
	for (GE::ECS::Entity childEntity : entity.GetChildren())
	{
		Propergate(childEntity, entity.m_worldTransform);
	}	
}