#include <pch.h>
#include <Systems/Physics/CollisionSystem.h>
#include <Graphics/GraphicsEngine.h>
#include <Audio/AudioEngine.h>

using namespace GE::Math;

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;
using namespace Input;
using namespace Graphics;

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
	std::set<Entity>& list = GetUpdatableEntities();

	for (Entity entity : list)
	{
		BoxCollider* updateEntity = m_ecs->GetComponent<BoxCollider>(entity);
		Transform* newCenter = m_ecs->GetComponent<Transform>(entity);
		UpdateAABB(*updateEntity, newCenter->m_parentWorldTransform * dVec4(newCenter->m_pos, 1.0));
		updateEntity->Render();
	}

	//spatial partitioning -> uniform grid
	CreatePartitions(m_rowsPartition, m_colsPartition);
	for (Partition& partition : m_partitions)
	{
		auto& gEngine{ Graphics::GraphicsEngine::GetInstance() };
		//drawing partition's border
		gEngine.DrawLine(partition.min, { partition.max.x, partition.min.y });
		gEngine.DrawLine({ partition.max.x, partition.min.y }, partition.max);

		if (partition.m_entitiesInPartition.empty()) 
		{
			continue;
		}

		for (Entity& entity1 : partition.m_entitiesInPartition)
		{
			//mouse click check
			BoxCollider* entity1Col = m_ecs->GetComponent<BoxCollider>(entity1);
			dVec2 mousePos{};

			InputManager* input = &(GE::Input::InputManager::GetInstance());
			mousePos = input->GetMousePosWorld();

			entity1Col->m_mouseCollided = Collide(*entity1Col, mousePos);

			//obj collide check
			for (Entity entity2 : partition.m_entitiesInPartition)
			{
				if (entity1 == entity2)
				{
					continue;
				}

				BoxCollider* entity2Col = m_ecs->GetComponent<BoxCollider>(entity2);

				if (Collide(*entity1Col, *entity2Col))
				{
					entity1Col->m_collided.insert(entity2Col);
					//Audio::AudioEngine::GetInstance().PlaySound(Assets::AssetManager::GetInstance().GetSound("damage_taken"), 1.50f);
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
}

void CollisionSystem::UpdateAABB(BoxCollider& entity, const dVec2& newCenter)
{
	entity.m_center = newCenter;
	entity.m_min.x = entity.m_center.x - entity.m_width / 2.0f;
	entity.m_min.y = entity.m_center.y - entity.m_height / 2.0f;
	entity.m_max.x = entity.m_center.x + entity.m_width / 2.0f;
	entity.m_max.y = entity.m_center.y + entity.m_height / 2.0f;
}

void CollisionSystem::CreatePartitions(int rows, int cols)
{
	m_partitions.clear();
	GraphicsEngine& getWindowSize = GraphicsEngine::GetInstance();
	int partitionHeight = getWindowSize.GetVPHeight() / rows;
	int partitionWidth = getWindowSize.GetVPWidth() / cols;

	for (int row{}; row < m_rowsPartition; ++row)
	{
		for (int col{}; col < m_colsPartition; ++col)
		{
			Partition partition;
			partition.min = { -(getWindowSize.GetVPWidth() / 2.0) + static_cast<double>(partitionWidth * col) , -(getWindowSize.GetVPHeight() / 2.0) + static_cast<double>(partitionHeight * (row))};
			partition.max = { -(getWindowSize.GetVPWidth() / 2.0) + static_cast<double>(partitionWidth * (col + 1)), -(getWindowSize.GetVPHeight() / 2.0) + static_cast<double>(partitionHeight * (row + 1)) };

			std::set<Entity>& list = GetUpdatableEntities();
			for (Entity entity : list)
			{
				BoxCollider* pos = m_ecs->GetComponent<BoxCollider>(entity);
				if (!(pos->m_max.y < partition.min.y || pos->m_min.x > partition.max.x || pos->m_max.x < partition.min.x || pos->m_min.y > partition.max.y))
				{
					partition.m_entitiesInPartition.push_back(entity);
				}
			}
			m_partitions.push_back(std::move(partition));
		}
	}
}