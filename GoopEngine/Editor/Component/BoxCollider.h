#pragma once
#include <pch.h>

namespace GE
{
	namespace Component
	{
		struct AABB : public GE::ECS::Component
		{
			AABB(Math::dVec2 const& center, double width, double height) : m_width{ width }, m_height{ height },
				m_min{ center.x - width / 2.0f, center.y - height / 2.0f },
				m_max{ center.x + width / 2.0f, center.y + height / 2.0f }, m_center{ center },
				m_collided{ nullptr },
				m_mouseCollided{ false }
			{

			}

			double m_width;
			double m_height;
			Math::dVec2 m_min;
			Math::dVec2 m_max;
			Math::dVec2 m_center;
			//can use AABB & position(center)
			//GE::ECS::Entity * m_collided;
			std::set<AABB*> m_collided; //a set list of entities' pointers that current entity is collided with
			bool m_mouseCollided;
		};
	}
}