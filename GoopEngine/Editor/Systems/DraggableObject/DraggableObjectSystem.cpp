#include <DraggableObject/DraggableObjectSystem.h>
#include <Component/BoxCollider.h>
#include <Physics/CollisionSystem.h>
#include <Component/Transform.h>
#include <Component/Velocity.h>
#include <Component/Gravity.h>
#include "../../Events/InputEvents.h"

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;
using namespace Math;
using namespace Input;


void DraggableObjectSystem::Update() {
	for (Entity entity : m_entities)
	{
		//mouse click check
		BoxCollider* entity1Col = m_ecs->GetComponent<BoxCollider>(entity);

		InputManager* input = &(InputManager::GetInstance());
		static bool dragging = false;
		if (dragging)
		{
			Transform* transform = m_ecs->GetComponent<Transform>(entity);
			Gravity* gravity = m_ecs->GetComponent<Gravity>(entity, true);
			Velocity* velocity = m_ecs->GetComponent<Velocity>(entity, true);

			// check if should release
			if (!isDragging)
			{
				if (gravity)
				{
					gravity->SetActive(true);
				}
				if (velocity)
				{
					velocity->SetActive(true);
				}

				dragging = false;
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
				Gravity* gravity = m_ecs->GetComponent<Gravity>(entity, true);
				Velocity* velocity = m_ecs->GetComponent<Velocity>(entity, true);

				InputManager* input = &(InputManager::GetInstance());
				if (isDragging)
				{
					if (gravity)
					{
						gravity->SetActive(false);
					}
					if (velocity)
					{
						velocity->SetActive(false);
					}

					dragging = true;
					transform->m_pos = input->GetMousePosWorld();
				}
			}
		}

		//Transform* transform = m_ecs->GetComponent<Transform>(entity);
		//Gravity* gravity = m_ecs->GetComponent<Gravity>(entity);
		//Velocity* velocity = m_ecs->GetComponent<Velocity>(entity);

		//dVec2 mousePos{};

		//InputManager* input = &(InputManager::GetInstance());
		//if (input->IsKeyHeld(GPK_MOUSE_LEFT))
		//{
		//	mousePos = input->GetMousePosWorld();
		//	if (entity1Col->m_mouseCollided)
		//	{
		//		gravity->SetActive(false);
		//		velocity->SetActive(false);

		//		dVec2 prevObjPos = transform->m_pos;
		//		dVec2 currMousePos = mousePos;
		//		dVec2 newMousePos = input->GetMousePosWorld();
		//		dVec2 distTravelled = newMousePos - currMousePos;

		//		std::cout << "prevObjPos: " << prevObjPos << std::endl;
		//		std::cout << "currMousePos: " << currMousePos << std::endl;
		//		std::cout << "newMousePos: " << newMousePos << std::endl;
		//		std::cout << "distTravelled: " << distTravelled << std::endl;

		//		transform->m_pos = prevObjPos + distTravelled;
		//	}
		//}
		//else
		//{
		//	gravity->SetActive(true);
		//	velocity->SetActive(true);
		//}
	}
}

void DraggableObjectSystem::HandleEvent(Events::Event const* event)
{
	if (event->GetCategory() == Events::EVENT_TYPE::MOUSE_HELD)
	{
		if (static_cast<Events::MouseHeldEvent const*>(event)->GetKey() == GPK_MOUSE_LEFT) 
		{ 
			isDragging = true; 
			#ifdef _DEBUG
			std::cout << event->GetName() + " Event handled\n";
			#endif
		}
	}
	else if (event->GetCategory() == Events::EVENT_TYPE::MOUSE_RELEASED)
	{
		if (static_cast<Events::MouseReleasedEvent const*>(event)->GetKey() == GPK_MOUSE_LEFT)
		{
			isDragging = false;
			#ifdef _DEBUG
			std::cout << event->GetName() + " Event handled\n";
			#endif
		}
	}
}
