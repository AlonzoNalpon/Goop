/*!*********************************************************************
\file   PostRootTransformSystem.cpp
\author w.chinkitbryan\@digipen.edu
\date   29-October-2023
\brief
  2nd pass of computing transforms. This pass converts local transfroms
	to world transforms.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "PostRootTransformSystem.h"
#include <Component/Transform.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "TransformSystemHelper.h"
#include <Graphics/GraphicsEngine.h>
#include <Component/Sprite.h>

void GE::Systems::PostRootTransformSystem::LateUpdate()
{
	auto& frc = GE::FPS::FrameRateController::GetInstance();
	frc.StartSystemTimer();
	for (GE::ECS::Entity entity : m_ecs->GetEntities())
	{
		// This is a root entity
		if (m_ecs->GetParentEntity(entity) == GE::ECS::INVALID_ID)
		{
			// Assign own world transformation matrix
			Math::dMat4 identity
			{
				{ 1, 0, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 1 }
			};

			// Update recursively using entity's world transformation matrix
			Propergate(entity, identity);
		}
	}
	frc.EndSystemTimer("LocalToWorld Transform");
}

void GE::Systems::PostRootTransformSystem::Propergate(GE::ECS::Entity& entity, const Math::dMat4& parentWorldTrans)
{
	static GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	GE::Component::Transform* trans = ecs.GetComponent<GE::Component::Transform>(entity);

	if (trans == nullptr)
	{
		throw GE::Debug::Exception<PostRootTransformSystem>(GE::Debug::LEVEL_CRITICAL, ErrMsg("entity " + std::to_string(entity) + " is missing a transform component. All entities must have a transform component!!"));
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
		trans->m_worldPos = parentWorldTrans * GE::Math::dVec4(trans->m_pos, 1);
		trans->m_worldScale = trans->m_scale * parentTrans->m_scale;
		trans->m_worldRot = trans->m_rot + parentTrans->m_rot;
	}
	else
	{
		trans->m_worldPos = trans->m_pos;
		trans->m_worldScale = trans->m_scale;
		trans->m_worldRot = trans->m_rot;
	}

	Math::dMat4 T
	{
		{ 1, 0, 0, trans->m_worldPos.x },
		{ 0, 1, 0, trans->m_worldPos.y },
		{ 0, 0, 1, trans->m_worldPos.z },
		{ 0, 0, 0, 1 }
	};
	Math::dMat4 S
	{
		{ trans->m_worldScale.x, 0, 0, 0 },
		{ 0, trans->m_worldScale.y, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	double rad = (trans->m_worldRot.z / 180.0) * M_PI;
	Math::dMat4 Z
	{
		{ std::cos(rad), -std::sin(rad), 0, 0 },
		{ std::sin(rad), std::cos(rad), 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	rad = (trans->m_worldRot.y / 180.0) * M_PI;
	Math::dMat4 Y
	{
		{ std::cos(rad), std::sin(rad), 0, 0 },
		{ 0, 1, 0, 0 },
		{ -std::sin(rad), 0, std::cos(rad), 0 },
		{ 0, 0, 0, 1 }
	};
	rad = (trans->m_worldRot.x / 180.0) * M_PI;
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
		scaleX = sprite->m_spriteData.info.width;			
		scaleY = sprite->m_spriteData.info.height;

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
