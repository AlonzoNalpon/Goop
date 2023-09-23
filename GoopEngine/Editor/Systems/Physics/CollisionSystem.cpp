#include <Physics/CollisionSystem.h>

using namespace GE::Math;

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

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
	for (Entity entity : m_entities) 
	{
		//Velocity* updateVel = m_ecs->GetComponent<Velocity>(entity);

		
		BoxCollider* updateEntity = m_ecs->GetComponent<BoxCollider>(entity);
		Transform* newCenter = m_ecs->GetComponent<Transform>(entity);
		if (!updateEntity) {
			std::cout << "updateEntity ERROR\n";
		}
		if (!newCenter) {
			std::cout << "newCenter ERROR\n";
		}
		//std::cout << "Centre: " << newCenter->m_pos << std::endl;
		UpdateAABB(*updateEntity, newCenter->m_pos, *newCenter);
	}
	//loop through every entity & check against every OTHER entity if they collide either true
	for (Entity entity1 : m_entities)
	{
		BoxCollider* entity1Col = m_ecs->GetComponent<BoxCollider>(entity1);
		//entity1Col->m_collided.clear();

		entity1Col->Render();

		//GE::Math::dVec2 mouse1{ 3, 1 }; //should collide
		//GE::Math::dVec2 mouse2{ 5, 4 };	//shouldnt collide

		//if (entity1Col->m_mouseCollided = Collide(*entity1Col, mouse1)) {
		//	std::cout << "Collided." << std::endl;
		//	std::cout << "Box coordinates: " << entity1Col->m_center << std::endl;
		//	std::cout << "Point coordinates: " << mouse1 << std::endl;
		//}
		//else {
		//	std::cout << "Not collided." << std::endl;
		//	std::cout << "Box coordinates: " << entity1Col->m_center << std::endl;
		//	std::cout << "Point coordinates: " << mouse1 << std::endl;
		//}

		//if (entity1Col->m_mouseCollided = Collide(*entity1Col, mouse2)) {
		//	std::cout << "Collided." << std::endl;
		//	std::cout << "Box coordinates: " << entity1Col->m_center << std::endl;
		//	std::cout << "Point coordinates: " << mouse2 << std::endl;
		//}
		//else {
		//	std::cout << "Not collided." << std::endl;
		//	std::cout << "Box coordinates: " << entity1Col->m_center << std::endl;
		//	std::cout << "Point coordinates: " << mouse2 << std::endl;
		//}

		for (Entity entity2 : m_entities) {
			if (entity1 == entity2) {
				continue;
			}

			BoxCollider* entity2Col = m_ecs->GetComponent<BoxCollider>(entity2);
			if (Collide(*entity1Col, *entity2Col)) {
				entity1Col->m_collided.insert(entity2Col);
				std::cout << "Collided." << std::endl;
				std::cout << "1st coordinates: " << entity1Col->m_center << std::endl;
				std::cout << "2nd coordinates: " << entity2Col->m_center << std::endl;
			}
			else {
				if (entity1Col->m_collided.count(entity2Col)) {
					entity1Col->m_collided.erase(entity2Col);
				}
				/*std::cout << "Not collided." << std::endl;
				std::cout << "1st coordinates: " << entity1Col->m_center << std::endl;
				std::cout << "2nd coordinates: " << entity2Col->m_center << std::endl;*/
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