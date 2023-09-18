#include <pch.h>

struct AABB : public GE::ECS::Component
{
	AABB(vec2 center, double width, double height)
	{
		m_center = center;
		m_width = width;
		m_height = height;
		m_min.x = centre.x - width / 2.0f;
		m_min.y = centre.y - height / 2.0f;
		m_max.x = centre.x + width / 2.0f;
		m_max.y = centre.y + height / 2.0f;
	}

	double m_width;
	double m_height;
	vec2 m_min;
	vec2 m_max;
	vec2 m_center;
	//can use AABB & position(center)
	Entity* m_collided;
	bool m_mouseCollided;
};