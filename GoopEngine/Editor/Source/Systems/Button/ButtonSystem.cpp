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
			GE::ECS::System::Start();
			m_buttonEntity = GE::ECS::INVALID_ID;
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

		void ButtonSystem::HandleEvent(Events::Event* event)
		{
			bool triggered = false;
			for (GE::ECS::Entity entity : m_entities)
			{
				GE::Component::GE_Button* entityButton = m_ecs->GetComponent<GE::Component::GE_Button>(entity);

				if (event->GetCategory() == Events::EVENT_TYPE::MOUSE_TRIGGERED)
				{
					if (static_cast<Events::MouseTriggeredEvent*>(event)->GetKey() == GPK_MOUSE_LEFT)
					{
						GE::Component::BoxCollider* entity1Col = m_ecs->GetComponent<GE::Component::BoxCollider>(entity);
						GE::Component::GE_Button* btn = m_ecs->GetComponent<GE::Component::GE_Button>(entity);

						if (entity1Col->m_mouseCollided)
						{
							triggered = true;
							switch (btn->m_eventType)
							{
							case GE::Component::GE_Button::NO_EVENT:
								break;
							case GE::Component::GE_Button::SELECT_CARD:
								//ECS::Entity cardHolder = m_ecs->GetComponent<>
								break;
							case GE::Component::GE_Button::UNSELECT_CARD:
								break;
							case GE::Component::GE_Button::CHANGE_SCENE:
								GE::GSM::GameStateManager::GetInstance().SetNextScene(entityButton->m_param);
								break;
							case GE::Component::GE_Button::UNPAUSE:
							{
								GE::ECS::Entity pauseMenu{ std::stoul(btn->m_param) };
								m_ecs->SetIsActiveEntity(pauseMenu, false);
								break;
							}
							default:
								break;
							}
						}
					}
				}
				if (triggered)
					break;
			}
		}
	}
}