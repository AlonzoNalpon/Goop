/*!*********************************************************************
\file   PreRootTransformSystem.h
\author w.chinkitbryan\@digipen.edu
\date   29-October-2023
\brief
	1st pass of computing transforms. This pass converts world tramsfroms
	to local transforms.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "PreRootTransformSystem.h"
#include <Component/Transform.h>
#include "TransformSystemHelper.h"

void GE::Systems::PreRootTransformSystem::Update()
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

void GE::Systems::PreRootTransformSystem::Propergate(GE::ECS::EntityComponentSystem& ecs, GE::ECS::Entity& entity, const Math::dMat4& parentWorldTrans)
{
	GE::Component::Transform* trans = ecs.GetComponent<GE::Component::Transform>(entity);

	if (trans == nullptr)
	{
		throw GE::Debug::Exception<PreRootTransformSystem>(GE::Debug::LEVEL_CRITICAL, ErrMsg("entity " + std::to_string(entity) + " is missing a transform component. All entities must have a transform component!!"));
	}

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
		trans->m_pos = parentWorldTrans * GE::Math::dVec4(trans->m_localPos, 1);
		trans->m_scale = trans->m_localScale * parentTrans->m_scale;
		trans->m_rot = trans->m_localRot + parentTrans->m_rot;
		trans->m_rot = GE::Math::Wrap(trans->m_rot, 0, 360);
	}
	else
	{
		trans->m_pos = trans->m_localPos;
		trans->m_scale = trans->m_localScale;
		trans->m_rot = trans->m_localRot;
	}

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
