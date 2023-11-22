#include <pch.h>
#include <Systems/Button/ButtonSystem.h>
// #include <Component/Button.h>
#include <Events/InputEvents.h>
#include <GameStateManager/GameStateManager.h>
#include <InputManager/InputManager.h>

namespace GE
{
	namespace Systems
	{
		void ButtonSystem::Start()
		{

		}

		void ButtonSystem::Update()
		{
			for (GE::ECS::Entity entity : GetUpdatableEntities())
			{
				GE::Component::BoxCollider* entity1Col = m_ecs->GetComponent<GE::Component::BoxCollider>(entity);
				// static GE::Input::InputManager& input = GE::Input::InputManager::GetInstance();

				if (entity1Col->m_mouseCollided)
				{
					std::cout << "SPECIFIC CLICK DETECTED!!" << std::endl;
				}
			}
		}

		void ButtonSystem::HandleEvent(Events::Event const* event)
		{
			for (GE::ECS::Entity entity : m_entities)
			{
				GE::Component::GE_Button* entityButton = m_ecs->GetComponent<GE::Component::GE_Button>(entity);

				if (event->GetCategory() == Events::EVENT_TYPE::MOUSE_TRIGGERED)
				{
					if (static_cast<Events::MouseTriggeredEvent const*>(event)->GetKey() == GPK_MOUSE_LEFT)
					{
						GE::Component::BoxCollider* entity1Col = m_ecs->GetComponent<GE::Component::BoxCollider>(entity);

						if (entity1Col->m_mouseCollided)
						{
							GE::GSM::GameStateManager::GetInstance().SetNextScene(entityButton->m_nextScene);
							break;
						}
					}
				}
			}
		}
	}
}