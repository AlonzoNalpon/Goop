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

void GE::Systems::PostRootTransformSystem::Start()
{
	Update();
}


void GE::Systems::PostRootTransformSystem::Update()
{
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
			Propergate(*m_ecs, entity, identity);
		}
	}
}

void GE::Systems::PostRootTransformSystem::Propergate(GE::ECS::EntityComponentSystem& ecs, GE::ECS::Entity& entity, const Math::dMat4& parentWorldTrans)
{
	GE::Component::Transform* trans = ecs.GetComponent<GE::Component::Transform>(entity);

	if (trans == nullptr)
	{
		throw GE::Debug::Exception<PostRootTransformSystem>(GE::Debug::LEVEL_CRITICAL, ErrMsg("entity " + std::to_string(entity) + " is missing a transform component. All entities must have a transform component!!"));
	}

	trans->m_parentWorldTransform = parentWorldTrans;

	/////////////////////////////////////
	// Setting local transformation data
	/////////////////////////////////////
	GE::Component::Transform* parentTrans;
	GE::ECS::Entity parent = ecs.GetParentEntity(entity);
	// If has parent, set my world transform to using parent
	if (parent != GE::ECS::INVALID_ID)
	{
		try
		{
			parentTrans = ecs.GetComponent<GE::Component::Transform>(parent);
			if (parentTrans == nullptr)
			{
				throw GE::Debug::Exception<PostRootTransformSystem>(GE::Debug::LEVEL_CRITICAL, ErrMsg("entity " + std::to_string(parent) + " is missing a transform component. All entities must have a transform component!!"));
			}
			GE::Math::dMat4 invWorldTransform;
			GE::Math::MtxInverse(invWorldTransform, trans->m_parentWorldTransform);

			trans->m_pos = invWorldTransform * GE::Math::dVec4(trans->m_worldPos, 1);
			trans->m_scale = trans->m_worldScale / parentTrans->m_worldScale;
			trans->m_rot = trans->m_worldRot - parentTrans->m_worldRot;
		}
		catch (GE::Debug::IExceptionBase& e)
		{
			e.LogSource();
		}
	}
	// no parent means local is world transform
	else
	{
		trans->m_pos = trans->m_worldPos;
		trans->m_scale = trans->m_worldScale;
		trans->m_rot = trans->m_worldRot;
	}

	Math::dMat4 T
	{
		{ 1, 0, 0, trans->m_pos.x },
		{ 0, 1, 0, trans->m_pos.y },
		{ 0, 0, 1, trans->m_pos.z },
		{ 0, 0, 0, 1 }
	};
	Math::dMat4 S
	{
		 { trans->m_scale.x, 0, 0, 0 },
		 { 0, trans->m_scale.y, 0, 0 },
		 { 0, 0, 1, 0 },
		 { 0, 0, 0, 1 }
	};
	double rad = trans->m_rot.z / 180.0 * M_PI;
	Math::dMat4 Z
	{
		{ std::cos(rad), -std::sin(rad), 0, 0 },
		{ std::sin(rad), std::cos(rad), 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	rad = trans->m_rot.y / 180.0 * M_PI;
	Math::dMat4 Y
	{
		{ std::cos(rad), std::sin(rad), 0, 0 },
		{ 0, 1, 0, 0 },
		{ -std::sin(rad), 0, std::cos(rad), 0 },
		{ 0, 0, 0, 1 }
	};
	rad = trans->m_rot.x / 180.0 * M_PI;
	Math::dMat4 X
	{
		{ 1, 0, 0, 0 },
		{ 0, std::cos(rad), -std::sin(rad), 0 },
		{ 0, std::sin(rad), std::cos(rad), 0 },
		{ 0, 0, 0, 1 }
	};

	trans->m_worldTransform = trans->m_parentWorldTransform * (T * (X * Y * Z) * S);

	std::set<GE::ECS::Entity>& m_children = ecs.GetChildEntities(entity);
	// End condition no children
	if (m_children.empty())
	{
		return;
	}

	// Update all children based ownself's new world transform
	for (GE::ECS::Entity childEntity : m_children)
	{
		Propergate(ecs, childEntity, trans->m_worldTransform);
	}	
}
