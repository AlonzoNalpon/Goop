#include <pch.h>
#include <Systems/Button/ButtonSystem.h>
// #include <Component/Button.h>
#include <Component/CardHolder.h>
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
			m_shouldHandle = false;
			for (GE::ECS::Entity entity : GetUpdatableEntities())
			{
				GE::Component::BoxCollider* entity1Col = m_ecs->GetComponent<GE::Component::BoxCollider>(entity);

				if (entity1Col->m_mouseCollided)
				{
					m_shouldHandle = true;
				}
			}
		}

		void ButtonSystem::HandleEvent(Events::Event* event)
		{
			// Only handle when system is running
			if (!m_shouldHandle)
			{
				return;
			}

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
							{
									// This requires a card component
									auto* card = m_ecs->GetComponent<Component::Card>(entity);
									if (!card) break; // no card -> bad behavior

									auto* cardHolder = m_ecs->GetComponent<Component::CardHolder>(card->tgtEntity);
									if (!cardHolder) break; // no holder -> bad behavior

									// Check if there's a free element
									for (auto& elem : cardHolder->elements)
									{
										if (elem.used) // skip if it's used!
											continue;

									
										elem.used = true; // now it's used!
									}
							}
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
							case GE::Component::GE_Button::NEXT_TURN:
							{
								GE::Events::EventManager::GetInstance().Dispatch(GE::Events::GameNextTurn());
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