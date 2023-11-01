#include <pch.h>
#include <Systems/DraggableObject/DraggableObjectSystem.h>
#include <Component/BoxCollider.h>
#include <Systems/Physics/CollisionSystem.h>
#include <Component/Transform.h>
#include "../../Events/InputEvents.h"
#include "../Physics/PhysicsSystem.h"

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;
using namespace Math;
using namespace Input;


void DraggableObjectSystem::Update() 
{
	auto& frc = GE::FPS::FrameRateController::GetInstance();
	frc.StartSystemTimer();
	for (Entity entity : GetUpdatableEntities())
	{
		//mouse click check
		BoxCollider* entity1Col = m_ecs->GetComponent<BoxCollider>(entity);

		InputManager* input = &(InputManager::GetInstance());
		static bool dragging = false;
		static Entity draggedEntity = INVALID_ID;
		if (dragging && entity == draggedEntity)
		{
			Transform* transform = m_ecs->GetComponent<Transform>(entity);

			// check if should release
			if (!isHeld)
			{
				dragging = false;
				draggedEntity = entity;
			}
			else // if dragging set mouse pos
			{
				transform->m_pos = input->GetMousePosWorld();
			}
		}
		else
		{
			if (entity1Col->m_mouseCollided)
			{
				Transform* transform = m_ecs->GetComponent<Transform>(entity);

				if (isHeld)
				{
					dragging = true;
					draggedEntity = entity;
					transform->m_pos = input->GetMousePosWorld();
				}
			}
		}
	}
	frc.EndSystemTimer("Draggable Objects");
}

void DraggableObjectSystem::HandleEvent(Events::Event const* event)
{
	if (event->GetCategory() == Events::EVENT_TYPE::MOUSE_HELD)
	{
		if (static_cast<Events::MouseHeldEvent const*>(event)->GetKey() == GPK_MOUSE_LEFT) 
		{ 
			isHeld = true;
		}
	}
	else if (event->GetCategory() == Events::EVENT_TYPE::MOUSE_RELEASED)
	{
		if (static_cast<Events::MouseReleasedEvent const*>(event)->GetKey() == GPK_MOUSE_LEFT)
		{
			isHeld = false;
		}
	}
}
