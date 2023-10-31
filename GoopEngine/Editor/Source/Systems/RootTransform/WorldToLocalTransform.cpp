/*!*********************************************************************
\file   WorldToLocalTransform.h
\author w.chinkitbryan\@digipen.edu
\date   29-October-2023
\brief
	Converts world transfroms	to local transforms.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "WorldToLocalTransfrom.h"
#include <Component/Transform.h>
#include "TransformSystemHelper.h"
#define _USE_MATH_DEFINES
#include <math.h>

void GE::Systems::WorldToLocalTransform::Propergate(GE::ECS::Entity& entity, GE::Math::dMat4& parentWorldTrans)
{
	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
	GE::Component::Transform* trans = ecs.GetComponent<GE::Component::Transform>(entity);

	if (trans == nullptr)
	{
		throw GE::Debug::Exception<WorldToLocalTransform>(GE::Debug::LEVEL_CRITICAL, ErrMsg("entity " + std::to_string(entity) + " is missing a transform component. All entities must have a transform component!!"));
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
				throw GE::Debug::Exception<WorldToLocalTransform>(GE::Debug::LEVEL_CRITICAL, ErrMsg("entity " + std::to_string(parent) + " is missing a transform component. All entities must have a transform component!!"));
			}
			GE::Math::dMat4 invWorldTransform;
			GE::Math::MtxInverse(invWorldTransform, trans->m_parentWorldTransform);

			GE::Math::dVec3 pos = invWorldTransform * GE::Math::dVec4(trans->GetWorldPos(), 1);
			GE::Math::dVec3 scale = trans->GetWorldScale() / parentTrans->GetWorldScale();
			GE::Math::dVec3 rot = trans->GetWorldRot() - parentTrans->GetWorldRot();
			trans->SetLocalTransform(entity, pos, scale, rot);
		}
		catch (GE::Debug::IExceptionBase& e)
		{
			e.LogSource();
		}
	}
	// no parent means local is world transform
	else
	{
		trans->SetLocalTransform(entity, trans->GetWorldPos(), trans->GetWorldScale(), trans->GetWorldRot());
	}

	Math::dMat4 T
	{
		{ 1, 0, 0, trans->GetPos().x },
		{ 0, 1, 0, trans->GetPos().y },
		{ 0, 0, 1, trans->GetPos().z },
		{ 0, 0, 0, 1 }
	};
	Math::dMat4 S
	{
		{ trans->GetScale().x, 0, 0, 0 },
		{ 0, trans->GetScale().y, 0, 0},
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	double rad = trans->GetRot().z / 180.0 * M_PI;
	Math::dMat4 Z
	{
		{ std::cos(rad), -std::sin(rad), 0, 0 },
		{ std::sin(rad), std::cos(rad), 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	rad = trans->GetRot().y / 180.0 * M_PI;
	Math::dMat4 Y
	{
		{ std::cos(rad), std::sin(rad), 0, 0 },
		{ 0, 1, 0, 0 },
		{ -std::sin(rad), 0, std::cos(rad), 0 },
		{ 0, 0, 0, 1 }
	};
	rad = trans->GetRot().x / 180.0 * M_PI;
	Math::dMat4 X
	{
		{ 1, 0, 0, 0 },
		{ 0, std::cos(rad), -std::sin(rad), 0 },
		{ 0, std::sin(rad), std::cos(rad), 0 },
		{ 0, 0, 0, 1 }
	};

	trans->m_worldTransform = (T * (X * Y * Z) * S);

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
