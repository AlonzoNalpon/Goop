#include <DraggableObject/DraggableObjectSystem.h>
#include <Component/BoxCollider.h>
#include <Physics/CollisionSystem.h>
#include <Component/Transform.h>
#include "../../Events/InputEvents.h"
#include "../Physics/PhysicsSystem.h"

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

			// check if should release
			if (!isHeld)
			{
				m_ecs->RegisterEntityToSystem<GE::Systems::PhysicsSystem>(entity);

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

				InputManager* input = &(InputManager::GetInstance());
				if (isHeld)
				{
					m_ecs->UnregisterEntityFromSystem<GE::Systems::PhysicsSystem>(entity);

					dragging = true;
					transform->m_pos = input->GetMousePosWorld();
				}
			}
		}
		//if (dragging)
		//{
		//	Transform* transform = m_ecs->GetComponent<Transform>(entity);
		//	Gravity* gravity = m_ecs->GetComponent<Gravity>(entity, true);
		//	Velocity* velocity = m_ecs->GetComponent<Velocity>(entity, true);

		//	// check if should release
		//	if (!isHeld)
		//	{
		//		if (gravity)
		//		{
		//			gravity->SetActive(true);
		//		}
		//		if (velocity)
		//		{
		//			velocity->SetActive(true);
		//		}

		//		dragging = false;
		//	}
		//	else // if dragging set mouse pos
		//	{
		//		transform->m_pos = input->GetMousePosWorld();
		//	}
		//}
		//else
		//{
		//	if (entity1Col->m_mouseCollided)
		//	{
		//		Transform* transform = m_ecs->GetComponent<Transform>(entity);
		//		Gravity* gravity = m_ecs->GetComponent<Gravity>(entity, true);
		//		Velocity* velocity = m_ecs->GetComponent<Velocity>(entity, true);

		//		InputManager* input = &(InputManager::GetInstance());
		//		if (isHeld)
		//		{
		//			if (gravity)
		//			{
		//				gravity->SetActive(false);
		//			}
		//			if (velocity)
		//			{
		//				velocity->SetActive(false);
		//			}

		//			dragging = true;
		//			transform->m_pos = input->GetMousePosWorld();
		//		}
		//	}
		//}

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
			isHeld = true;
			#ifdef _DEBUG
			std::cout << event->GetName() + " Event handled\n";
			#endif
		}
	}
	else if (event->GetCategory() == Events::EVENT_TYPE::MOUSE_RELEASED)
	{
		if (static_cast<Events::MouseReleasedEvent const*>(event)->GetKey() == GPK_MOUSE_LEFT)
		{
			isHeld = false;
			#ifdef _DEBUG
			std::cout << event->GetName() + " Event handled\n";
			#endif
		}
	}
}
