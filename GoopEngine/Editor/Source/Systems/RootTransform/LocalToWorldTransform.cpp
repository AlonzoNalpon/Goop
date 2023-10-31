/*!*********************************************************************
\file   LocalToWorldTransform.cpp
\author w.chinkitbryan\@digipen.edu
\date   29-October-2023
\brief
  Converts local transfroms	to world transforms.

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
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	GE::Component::Transform* trans = ecs.GetComponent<GE::Component::Transform>(entity);

	if (trans == nullptr)
	{
		throw GE::Debug::Exception<LocalToWorldTransform>(GE::Debug::LEVEL_CRITICAL, ErrMsg("entity " + std::to_string(entity) + " is missing a transform component. All entities must have a transform component!!"));
	}

	trans->m_parentWorldTransform = parentWorldTrans;

	/////////////////////////////////////
	// Setting world transformation data
	/////////////////////////////////////
	GE::Component::Transform* parentTrans;
	GE::ECS::Entity parent = ecs.GetParentEntity(entity);
	// If has parent, set my world transform to using parent
	// no parent means local is world transform
	if (parent != GE::ECS::INVALID_ID)
	{
		parentTrans = ecs.GetComponent<GE::Component::Transform>(parent);
		trans->SetRawWorldPos(parentWorldTrans * GE::Math::dVec4(trans->GetPos(), 1));
		trans->SetRawWorldScale(trans->GetScale() * parentTrans->GetScale());
		trans->SetRawWorldRot(trans->GetRot() + parentTrans->GetRot());
	}
	else
	{
		trans->SetRawWorldPos(trans->GetPos());
		trans->SetRawWorldScale(trans->GetScale());
		trans->SetRawWorldRot(trans->GetRot());
	}

	Math::dMat4 T
	{
		{ 1, 0, 0, trans->GetWorldPos().x },
		{ 0, 1, 0, trans->GetWorldPos().y },
		{ 0, 0, 1, trans->GetWorldPos().z },
		{ 0, 0, 0, 1 }
	};
	Math::dMat4 S
	{
		{ trans->GetWorldScale().x, 0, 0, 0 },
		{ 0, trans->GetWorldScale().y, 0, 0},
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	double rad = (trans->GetWorldRot().z / 180.0) * M_PI;
	Math::dMat4 Z
	{
		{ std::cos(rad), -std::sin(rad), 0, 0 },
		{ std::sin(rad), std::cos(rad), 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	rad = (trans->GetWorldRot().y / 180.0) * M_PI;
	Math::dMat4 Y
	{
		{ std::cos(rad), std::sin(rad), 0, 0 },
		{ 0, 1, 0, 0 },
		{ -std::sin(rad), 0, std::cos(rad), 0 },
		{ 0, 0, 0, 1 }
	};
	rad = (trans->GetWorldRot().x / 180.0) * M_PI;
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

		trans->m_renderTransform = resultMtx * textureScale * S;
	}

	trans->m_worldTransform = resultMtx * S;

	std::set<GE::ECS::Entity>& m_children = ecs.GetChildEntities(entity);
	// End condition no children
	if (m_children.empty())
	{
		return;
	}

	// Update all children based ownself's new world transform
	for (GE::ECS::Entity childEntity : m_children)
	{
		Propergate(childEntity, trans->m_worldTransform);
	}	
}
