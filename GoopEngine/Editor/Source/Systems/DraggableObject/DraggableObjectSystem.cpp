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


void GE::Systems::DraggableObjectSystem::Start()
{
	m_draggedEntity = INVALID_ID;
}

void DraggableObjectSystem::Update()
{
	auto& frc = GE::FPS::FrameRateController::GetInstance();
	frc.StartSystemTimer();
	for (Entity entity : GetUpdatableEntities())
	{
		//mouse click check
		BoxCollider* entity1Col = m_ecs->GetComponent<BoxCollider>(entity);

		static InputManager& input = InputManager::GetInstance();
		Transform* transform = m_ecs->GetComponent<Transform>(entity);
		
		if (entity == m_draggedEntity)
		{
			// check if should release
			if (!m_isHeld)
			{
				// Release dragged entity
				m_draggedEntity = INVALID_ID;
			}
			else // if dragging set mouse pos
			{
				m_draggedEntity = entity;
				transform->m_pos = input.GetMousePosWorld();
			}
		}
		else if (m_draggedEntity == INVALID_ID)
		{
			if (entity1Col->m_mouseCollided)
			{
				if (m_isHeld)
				{
					m_draggedEntity = entity;
					transform->m_pos = input.GetMousePosWorld();
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
			m_isHeld = true;
		}
	}
	else if (event->GetCategory() == Events::EVENT_TYPE::MOUSE_RELEASED)
	{
		if (static_cast<Events::MouseReleasedEvent const*>(event)->GetKey() == GPK_MOUSE_LEFT)
		{
			m_isHeld = false;
		}
	}
}
