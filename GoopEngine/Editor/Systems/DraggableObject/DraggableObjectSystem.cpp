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
		Transform* transform = m_ecs->GetComponent<Transform>(entity);
		Gravity* gravity = nullptr;
		Velocity* velocity = nullptr;

		if (m_ecs->GetComponent<Gravity>(entity) != NULL && m_ecs->GetComponent<Velocity>(entity) != NULL) {
			gravity = m_ecs->GetComponent<Gravity>(entity);
			velocity = m_ecs->GetComponent<Velocity>(entity);
		}

		dVec2 mousePos{};

		InputManager* input = &(InputManager::GetInstance());
		if (input->IsKeyHeld(GPK_MOUSE_LEFT))
		{
			mousePos = input->GetMousePosWorld();
			if (entity1Col->m_mouseCollided)
			{
				gravity->SetActive(false);
				velocity->SetActive(false);

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
		}

		if (!input->IsKeyHeld(GPK_MOUSE_LEFT)) {
			gravity->SetActive(true);
			velocity->SetActive(true);
		}

		entity1Col->Render();
	}
}