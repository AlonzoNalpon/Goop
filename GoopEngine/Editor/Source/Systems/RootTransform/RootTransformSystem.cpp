#include <pch.h>
#include "RootTransformSystem.h"
#include <Component/Transform.h>
#define _USE_MATH_DEFINES
#include <math.h>

void GE::Systems::RootTransformSystem::Update()
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
			Propergate(entity, identity);
		}
	}
}

void GE::Systems::RootTransformSystem::Propergate(GE::ECS::Entity& entity, const Math::dMat4& parentWorldTrans)
{
	// Compute own world transform matrix first
	GE::Component::Transform* trans = m_ecs->GetComponent<GE::Component::Transform>(entity);

	if (trans == nullptr)
		throw GE::Debug::Exception<RootTransformSystem>(GE::Debug::LEVEL_CRITICAL, ErrMsg("entity " + std::to_string(entity) + " is missing a transform component. All entities must have a transform component!!"));

	trans->m_parentWorldTransform = parentWorldTrans;

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

	std::set<GE::ECS::Entity>& m_children = m_ecs->GetChildEntities(entity);
	// End condition no children
	if (m_children.size() == 0)
	{
		return;
	}

	// Update all children based ownself's new world transform
	for (GE::ECS::Entity childEntity : m_children)
	{
		Propergate(childEntity, trans->m_worldTransform);
	}	
}
