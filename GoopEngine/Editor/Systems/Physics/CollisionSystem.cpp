#include <Physics/CollisionSystem.h>

using namespace GE::Math;

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;
using namespace Input;

//AABB & mouse input
bool CollisionSystem::Collide(BoxCollider& box, dVec2& input)
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
bool CollisionSystem::Collide(BoxCollider& box1, BoxCollider& box2)
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

void CollisionSystem::Update()
{
	for (Entity entity : GetUpdatableEntities())
	{
		BoxCollider* updateEntity = m_ecs->GetComponent<BoxCollider>(entity);
		Transform* newCenter = m_ecs->GetComponent<Transform>(entity);
		if (!updateEntity) 
		{
			std::cout << "updateEntity ERROR\n";
		}
		if (!newCenter) 
		{
			std::cout << "newCenter ERROR\n";
		}
		UpdateAABB(*updateEntity, newCenter->m_pos, *newCenter);
	}

	//loop through every entity & check against every OTHER entity if they collide either true
	for (Entity entity1 : GetUpdatableEntities())
	{
		//mouse click check
		BoxCollider* entity1Col = m_ecs->GetComponent<BoxCollider>(entity1);
		dVec2 mousePos{};

		InputManager* input = &(GE::Input::InputManager::GetInstance());
		mousePos = input->GetMousePosWorld();

		entity1Col->m_mouseCollided = Collide(*entity1Col, mousePos);

		entity1Col->Render();

		//obj collide check
		for (Entity entity2 : GetUpdatableEntities())
		{
			if (entity1 == entity2) 
			{
				continue;
			}

			BoxCollider* entity2Col = m_ecs->GetComponent<BoxCollider>(entity2);

			if (Collide(*entity1Col, *entity2Col)) 
			{
				entity1Col->m_collided.insert(entity2Col);
				/*#ifdef _DEBUG
				std::cout << "Collided." << std::endl;
				std::cout << "1st coordinates: " << entity1Col->m_center << std::endl;
				std::cout << "2nd coordinates: " << entity2Col->m_center << std::endl;
				#endif*/
			}

			else 
			{
				if (entity1Col->m_collided.count(entity2Col)) 
				{
					entity1Col->m_collided.erase(entity2Col);
				}
			}
		}
	}
}

void CollisionSystem::UpdateAABB(BoxCollider& entity, const dVec2& newCenter, Transform& scale)
{
	entity.m_center = newCenter;
	entity.m_min.x = entity.m_center.x - (scale.m_scale.x * entity.m_width) / 2.0f;
	entity.m_min.y = entity.m_center.y - (scale.m_scale.y * entity.m_height) / 2.0f;
	entity.m_max.x = entity.m_center.x + (scale.m_scale.x * entity.m_width) / 2.0f;
	entity.m_max.y = entity.m_center.y + (scale.m_scale.y * entity.m_height) / 2.0f;
}