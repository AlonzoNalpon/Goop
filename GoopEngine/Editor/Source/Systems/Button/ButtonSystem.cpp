/*!*********************************************************************
\file   ButtonSystem.cpp
\author loh.j@digipen.edu
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
							{
								// A card has called select card button event:
								//	This requires caller to have a card component
								auto* card = m_ecs->GetComponent<Component::Card>(entity);
								int handIdx{ static_cast<int>(m_ecs->GetComponent<Component::CardHolderElem>(entity)->elemIdx) };
								if (!card) break; 
								if (card->cardID == Component::Card::NO_CARD)
								{
									GE::Debug::ErrorLogger::GetInstance().LogWarning("Trying to select a card with id: NO_CARD");
									break; // no card -> bad behavior
								}

								auto* cardHolder = m_ecs->GetComponent<Component::CardHolder>(card->tgtEntity);
								if (!cardHolder) break; // no holder -> bad behavior


								int elemIdx{};
								// Check if there's a free element
								for (auto& elem : cardHolder->elements)
								{
									if (elem.used) // skip if it's used!
									{
										++elemIdx;
										continue;
									}

									auto const* cardSprite = m_ecs->GetComponent<Component::Sprite>(entity);
									auto* elemSprite = m_ecs->GetComponent<Component::Sprite>(elem.elemEntity);

									elem.cardEntity = entity; // record card entity and image
									elem.spriteID = cardSprite->m_spriteData.texture; // tex ID but not the dimensions
									elem.defaultSpriteID = elemSprite->m_spriteData.texture; // set default tex ID
									m_ecs->SetIsActiveEntity(entity, false); // make the card inactive
									elem.used = true; // now it's used!
										
									// Visual update:
									elemSprite->m_spriteData.texture = elem.spriteID; // make the sprite now hold the card sprite

									// play sound randomize
									if (m_ecs->HasComponent<GE::Component::Audio>(entity))
									{
										GE::Component::Audio* entitySound = m_ecs->GetComponent<GE::Component::Audio>(entity);
										entitySound->PlayRandom();
									}

									 //MONO SECTION
									{
										GE::ECS::Entity player = elem.scriptEntity;
										Component::Scripts* scripts = m_ecs->GetComponent<Component::Scripts>(player);

										// Get player's stats script
										GE::MONO::ScriptInstance* statsScriptInst{};
										for (auto& scriptInst : scripts->m_scriptList)
										{
											if (scriptInst.m_scriptName == "Stats")
											{
												statsScriptInst = &scriptInst;
											}
										}
										if (!statsScriptInst)
										{
											GE::Debug::ErrorLogger::GetInstance().LogError("target player entity missing in button event SELECT_CARD!");
											break;
										}

										// Now we get the player's script functions
										//MonoMethod* SetCardInHand = mono_class_get_method_from_name(statsScriptInst->m_scriptClass, "SetCardInHand", 2);
										//MonoMethod* SetCardInQueue = mono_class_get_method_from_name(statsScriptInst->m_scriptClass, "SetCardInQueue", 2);
										
										//Component::Card::CardID blankCard{ Component::Card::NO_CARD };// we will replace hand card with this
										/*void* argsHand []{ &handIdx, &blankCard };
										void* argsQueue[]{ &elemIdx, &card->cardID };
										mono_runtime_invoke(SetCardInHand, statsScriptInst->m_classInst, argsHand, nullptr);
										mono_runtime_invoke(SetCardInQueue, statsScriptInst->m_classInst, argsQueue, nullptr);*/

										MonoMethod* QueueCard = mono_class_get_method_from_name(statsScriptInst->m_scriptClass, "QueueCard", 1);
										std::vector<void*> args{ &handIdx };
										mono_runtime_invoke(QueueCard, mono_gchandle_get_target(statsScriptInst->m_gcHandle), args.data(), nullptr);
									}
									// Don't need to increment cardIdx since we're ending the loop
									break; // we're done here: a card has been assigned
								}
							}
								break;
							case GE::Component::GE_Button::UNSELECT_CARD:
								// An element of cardholder should run this event
								//	Therefore, it needs CardHolderElem
							{
								auto* holderElem = m_ecs->GetComponent<Component::CardHolderElem>(entity);
								if (!holderElem) break;

								auto* cardHolder = m_ecs->GetComponent<Component::CardHolder>(holderElem->holder);
								if (!cardHolder) break; // no holder -> bad behavior

								// Break immediately if the used flag is false
								if (!cardHolder->elements[holderElem->elemIdx].used)
									break;
								// We set used flag to false and restore sprite
								auto* elemSprite = m_ecs->GetComponent<Component::Sprite>(entity);
								elemSprite->m_spriteData.texture = cardHolder->elements[holderElem->elemIdx].defaultSpriteID;
								cardHolder->elements[holderElem->elemIdx].used = false;

								// now enable the card to be unselected
								auto cardEntity = cardHolder->elements[holderElem->elemIdx].cardEntity;
								m_ecs->SetIsActiveEntity(cardEntity, true);

								// play sound randomize
								if (m_ecs->HasComponent<GE::Component::Audio>(entity))
								{
									GE::Component::Audio* entitySound = m_ecs->GetComponent<GE::Component::Audio>(entity);
									entitySound->PlayRandom();
								}

								// MONO SECTION
								{
								 
									GE::ECS::Entity player = cardHolder->elements[holderElem->elemIdx].scriptEntity;
									Component::Scripts* scripts = m_ecs->GetComponent<Component::Scripts>(player);

									// Get player's stats script
									GE::MONO::ScriptInstance* statsScriptInst{};
									for (auto& scriptInst : scripts->m_scriptList)
									{
										if (scriptInst.m_scriptName == "Stats")
										{
											statsScriptInst = &scriptInst;
										}
									}
									if (!statsScriptInst)
									{
										GE::Debug::ErrorLogger::GetInstance().LogError("target player entity missing in button event SELECT_CARD!");
										break;
									}

									// Now we get the player's script functions
									int handIdx{ static_cast<int>(holderElem->elemIdx) }; // get the player entity ID
									std::cout << "UnqueueCard(" << handIdx << ")\n";
									//auto* cardComp = m_ecs->GetComponent<Component::Card>(cardEntity);
									//MonoMethod* SetCardInHand = mono_class_get_method_from_name(statsScriptInst->m_scriptClass, "SetCardInHand", 2);
									//MonoMethod* SetCardInQueue = mono_class_get_method_from_name(statsScriptInst->m_scriptClass, "SetCardInQueue", 2);

									//Component::Card::CardID blankCard{ Component::Card::NO_CARD };// we will replace hand card with this
									//void* argsHand[]	{ &handIdx, &cardComp->cardID };
									//void* argsQueue[] { &holderElem->elemIdx, &blankCard };
									//mono_runtime_invoke(SetCardInHand, mono_gchandle_get_target(statsScriptInst->m_gcHandle), argsHand, nullptr);
									//mono_runtime_invoke(SetCardInQueue, mono_gchandle_get_target(statsScriptInst->m_gcHandle), argsQueue, nullptr);

									MonoMethod* UnqueueCard = mono_class_get_method_from_name(statsScriptInst->m_scriptClass, "UnqueueCard", 1);
									std::vector<void*> args{ &handIdx };
									mono_runtime_invoke(UnqueueCard, mono_gchandle_get_target(statsScriptInst->m_gcHandle), args.data(), nullptr);
								}
								break;
							}
							case GE::Component::GE_Button::CHANGE_SCENE:
								GE::GSM::GameStateManager::GetInstance().SetNextScene(entityButton->m_param);
								break;
							case GE::Component::GE_Button::UNPAUSE:
							{
								GE::ECS::Entity pauseMenu{ std::stoul(btn->m_param) };
								m_ecs->SetIsActiveEntity(pauseMenu, false);
								m_ecs->GetSystem<GE::Systems::GameSystem>()->FlipPauseBool();
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