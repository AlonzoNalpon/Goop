#include "Collision.h"
#include "Transform.h"

using namespace GE;
using namespace GE::ECS;
using namespace GE::Collision;
//AABB & mouse input
bool CollisionSystem::Collide(AABB& box, dVec2& input)
{
	if (input.x <= box.m_max.x && input.x >= box.m_min.x)
	{
		if (input.y <= box.m_max.y && input.y >= box.m_min.y)
		{
			return true;
		}
	}
	return false;
}

//AABB & AABB
bool CollisionSystem::Collide(AABB& box1, AABB& box2)
{
	if (box1.m_min.x <= box2.m_max.x && box1.m_max.x >= box2.m_min.x)
	{
		if (box1.m_min.y <= box2.m_max.y && box1.m_max.y >= box2.m_min.y)
		{
			return true;
		}
	}
	return false;
}

void CollisionSystem::Awake()
{
	m_ecs = &EntityComponentSystem::GetInstance();
}

void CollisionSystem::Update()
{
	for (Entity entity : m_entities) 
	{
		//Velocity* updateVel = m_ecs->GetComponent<Velocity>(entity);

		
		AABB* updateEntity = m_ecs->GetComponent<AABB>(entity);
		Transform* newCenter = m_ecs->GetComponent<Transform>(entity);
		UpdateAABB(*updateEntity, newCenter->m_pos);
	}
	//loop through every entity & check against every OTHER entity if they collide either true
	for (Entity entity1 : m_entities)
	{
		AABB* entity1Col = m_ecs->GetComponent<AABB>(entity1);
		dVec2 temp{ 0, 0 };
		entity1Col->m_mouseCollided = Collide(*entity1Col, temp);

		for (Entity entity2 : m_entities) {
			if (entity1 == entity2) {
				continue;
			}

			AABB* entity2Col = m_ecs->GetComponent<AABB>(entity2);
			entity1Col->m_collided = Collide(*entity1Col, *entity2Col) ? &entity2 : nullptr;
		}
	}
}

void CollisionSystem::UpdateAABB(AABB& entity, const dVec2& newCenter)
{
	entity.m_center = newCenter;
	entity.m_min.x = entity.m_center.x - entity.m_width / 2.0f;
	entity.m_min.y = entity.m_center.y - entity.m_height / 2.0f;
	entity.m_max.x = entity.m_center.x + entity.m_width / 2.0f;
	entity.m_max.y = entity.m_center.y + entity.m_height / 2.0f;
}