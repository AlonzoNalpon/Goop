/*!*********************************************************************
\file   PreRootTransformSystem.h
\author w.chinkitbryan\@digipen.edu
\date   29-October-2023
\brief
	Converts world transfroms	to local transforms.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "WorldToLocalTransform.h"
#include <Component/Transform.h>
#include "TransformSystemHelper.h"
#define _USE_MATH_DEFINES
#include <math.h>

void GE::Systems::WorldToLocalTransform::Propergate(GE::ECS::Entity& entity, GE::Math::dMat4& parentWorldTrans)
{
	static GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();

	entity.m_parentWorldTransform = parentWorldTrans;

	/////////////////////////////////////
	// Setting local transformation data
	/////////////////////////////////////
	// If has parent, set my world transform to using parent
	if (entity.GetParent() != GE::ECS::INVALID_ID)
	{
		try
		{
			GE::Math::dMat4 invWorldTransform;
			GE::Math::MtxInverse(invWorldTransform, entity.m_parentWorldTransform);

			entity.m_pos.GetRaw() = invWorldTransform * GE::Math::dVec4(entity.m_worldPos, 1);
			entity.m_scale.GetRaw() = entity.m_worldScale.GetRaw() / entity.GetParent().m_worldScale.GetRaw();
			entity.m_rot.GetRaw() = entity.m_worldRot.GetRaw() - entity.GetParent().m_worldRot.GetRaw();
		}
		catch (GE::Debug::IExceptionBase& e)
		{
			e.LogSource();
		}
	}
	// no parent means local is world transform
	else
	{
		entity.m_pos = entity.m_worldPos;
		entity.m_scale = entity.m_worldScale;
		entity.m_rot = entity.m_worldRot;
	}

	Math::dMat4 T
	{
		{ 1, 0, 0, entity.m_pos.GetRaw().x },
		{ 0, 1, 0, entity.m_pos.GetRaw().y },
		{ 0, 0, 1, entity.m_pos.GetRaw().z },
		{ 0, 0, 0, 1 }
	};
	Math::dMat4 S
	{
		{ entity.m_scale.GetRaw().x, 0, 0, 0 },
		{ 0, entity.m_scale.GetRaw().y, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	double rad = entity.m_rot.GetRaw().z / 180.0 * M_PI;
	Math::dMat4 Z
	{
		{ std::cos(rad), -std::sin(rad), 0, 0 },
		{ std::sin(rad), std::cos(rad), 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	rad = entity.m_rot.GetRaw().y / 180.0 * M_PI;
	Math::dMat4 Y
	{
		{ std::cos(rad), std::sin(rad), 0, 0 },
		{ 0, 1, 0, 0 },
		{ -std::sin(rad), 0, std::cos(rad), 0 },
		{ 0, 0, 0, 1 }
	};
	rad = entity.m_rot.GetRaw().x / 180.0 * M_PI;
	Math::dMat4 X
	{
		{ 1, 0, 0, 0 },
		{ 0, std::cos(rad), -std::sin(rad), 0 },
		{ 0, std::sin(rad), std::cos(rad), 0 },
		{ 0, 0, 0, 1 }
	};

	entity.m_worldTransform = (T * (X * Y * Z) * S);

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
