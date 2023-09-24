#include <DraggableObject/DraggableObjectSystem.h>
#include <Component/BoxCollider.h>
#include <Physics/CollisionSystem.h>
#include <Component/Transform.h>
#include <Component/Velocity.h>
#include <Component/Gravity.h>

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

		if (entity1Col->m_mouseCollided)
		{
			Transform* transform = m_ecs->GetComponent<Transform>(entity);
			Gravity* gravity = m_ecs->GetComponent<Gravity>(entity, true);
			Velocity* velocity = m_ecs->GetComponent<Velocity>(entity, true);

			dVec2 mousePos{};
			InputManager* input = &(InputManager::GetInstance());
			if (input->IsKeyHeld(GPK_MOUSE_LEFT))
			{
				if (gravity)
				{
					gravity->SetActive(false);
				}
				if (velocity)
				{
					velocity->SetActive(false);
				}

				dVec2 prevObjPos = transform->m_pos;
				dVec2 currMousePos = mousePos;
				dVec2 newMousePos = input->GetMousePosWorld();
				dVec2 distTravelled = newMousePos - currMousePos;

				std::cout << "prevObjPos: " << prevObjPos << std::endl;
				std::cout << "currMousePos: " << currMousePos << std::endl;
				std::cout << "newMousePos: " << newMousePos << std::endl;
				std::cout << "distTravelled: " << distTravelled << std::endl;

				transform->m_pos = prevObjPos + distTravelled;
			}
			else if (input->IsKeyReleased(GPK_MOUSE_LEFT))
			{
				if (gravity)
				{
					gravity->SetActive(true);
				}
				if (velocity)
				{
					velocity->SetActive(true);
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