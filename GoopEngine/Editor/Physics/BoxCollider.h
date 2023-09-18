#pragma once
#include <pch.h>
//#include "../Math/GEM.h"
//#include "../ECS/Component/Component.h"
//#include "../ECS/EntityComponentSystem.h"

using namespace GE::Math;

struct AABB //: public GE::ECS::Component
{
	AABB(dVec2 center, double width, double height)
	{
		m_center = center;
		m_width = width;
		m_height = height;
		m_min.x = center.x - width / 2.0f;
		m_min.y = center.y - height / 2.0f;
		m_max.x = center.x + width / 2.0f;
		m_max.y = center.y + height / 2.0f;
	}

	double m_width;
	double m_height;
	dVec2 m_min;
	dVec2 m_max;
	dVec2 m_center;
	//can use AABB & position(center)
	GE::ECS::Entity * m_collided;
	bool m_mouseCollided;
};