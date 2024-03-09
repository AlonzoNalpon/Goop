/*!*********************************************************************
\file   ButtonSystem.cpp
\author loh.j\@digipen.edu
\date   26-November-2023
\brief
	System to allow entities to do something after being interacted
	with a click.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Systems/Button/ButtonSystem.h>
#include <Component/CardHolder.h>
#include <Events/InputEvents.h>
#include <GameStateManager/GameStateManager.h>
#include <InputManager/InputManager.h>
#include <Systems/GameSystem/GameSystem.h>
#include <ECS/System/System.h> // monomethod
#include <Component/Scripts.h>
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
			for (GE::ECS::Entity entity : GetUpdatableEntities())
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
#ifdef _DEBUG
								std::cout << "SELECT_CARD BUTTON EVENT SHOULD NO LONGER BE USED\n";
#endif
								break;
							case GE::Component::GE_Button::UNSELECT_CARD:
#ifdef _DEBUG
								std::cout << "UNSELECT_CARD BUTTON EVENT SHOULD NO LONGER BE USED\n";
#endif
								break;
							case GE::Component::GE_Button::CHANGE_SCENE:
								GE::GSM::GameStateManager::GetInstance().SetNextScene(entityButton->m_param);
								break;
							case GE::Component::GE_Button::UNPAUSE:
							{
								GE::ECS::Entity pauseMenu{ std::stoul(btn->m_param) };
								m_ecs->SetIsActiveEntity(pauseMenu, false);
								// m_ecs->GetSystem<GE::Systems::GameSystem>()->FlipPauseBool(); // DEPRECATED
								break;
							}
							case GE::Component::GE_Button::NEXT_TURN:
							{
								GE::Events::EventManager::GetInstance().Dispatch(GE::Events::GameNextTurn());
								break;
							}
							case GE::Component::GE_Button::QUIT_GAME:
							{
								GE::Events::EventManager::GetInstance().Dispatch(GE::Events::QuitGame());
								break;
							}
							case GE::Component::GE_Button::POPUP:
							{
								std::vector<std::string> multi_param = GoopUtils::SplitStringByPipe(btn->m_param);

								if (multi_param[0] == "Return")
								{
									GE::ECS::Entity hideMenu{ std::stoul(multi_param[1]) };
									m_ecs->SetIsActiveEntity(hideMenu, false);
								}
								else if (multi_param[0] == "Popup")
								{
									GE::ECS::Entity showMenu{ std::stoul(multi_param[1]) };
									m_ecs->SetIsActiveEntity(showMenu, true);
								}

								if (multi_param.size() > 2)
								{
									if (multi_param[2] == "Hide")
									{
										GE::ECS::Entity showMenu{ std::stoul(multi_param[3]) };
										m_ecs->SetIsActiveEntity(showMenu, false);
									}
									else if (multi_param[2] == "Show")
									{
										GE::ECS::Entity showMenu{ std::stoul(multi_param[3]) };
										m_ecs->SetIsActiveEntity(showMenu, true);
									}
								}
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