#include "RootTransformSystem.h"
#include <Component/Transform.h>
#define _USE_MATH_DEFINES
#include <math.h>

void GE::Systems::RootTransformSystem::Update()
{
	for (GE::ECS::Entity entity : m_entities)
	{
		// Assign own world transformation matrix
		Math::dMat3 identity
		{
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};

		// Update recursively using entity's world transformation matrix
		Propergate(entity, identity);
	}
}

void GE::Systems::RootTransformSystem::Propergate(GE::ECS::Entity& entity, const Math::dMat3& parentWorldTrans)
{
	// Compute own world transform matrix first
	GE::Component::Transform& trans = *m_ecs->GetComponent<GE::Component::Transform>(entity);

	Math::dMat3 T
	{
		1, 0, trans.m_pos.x,
		0, 1, trans.m_pos.y,
		0, 0, 1
	};
	Math::dMat3 S
	{
		trans.m_scale.x, 0, 0,
		0, trans.m_scale.y, 0,
		0, 0, 1
	};
	double rad = trans.m_rot / 180.0 * M_PI;
	Math::dMat3 R
	{
		std::cosh(rad), -std::sinh(rad), 0,
		-std::sinh(rad), std::cosh(rad), 0,
		0, 0, 1
	};

	trans.m_worldTransform = (T * R * S) * parentWorldTrans;

	// End condition no children
	if (trans.m_children.size() == 0)
	{
		return;
	}

	// Update all children based ownself's new world transform
	for (GE::ECS::Entity childEntity : trans.m_children)
	{
		Propergate(childEntity, trans.m_worldTransform);
	}
	
}
