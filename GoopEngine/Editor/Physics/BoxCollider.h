#include <pch.h>

struct AABB
{
	vec2 m_topLeft;
	vec2 m_topRight;
	vec2 m_botRight;
	vec2 m_botLeft;
	double m_width;
	double m_height;
	vec2 m_min;
	vec2 m_max;
	vec2 m_center;
	//can use AABB & position(center)
};