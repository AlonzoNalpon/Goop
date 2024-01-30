/*!*********************************************************************
\file   GameSystem.cpp
\author w.chinkitbryan\@digipen.edu
\date   26-November-2023
\brief  
  Single point interface where all our game logic will go through.
  A entity with the Game component with a GameManager.cs script will
  run and call all our game logic in 1 script which will then handle
  logic in scripts entirely enclosed in the C# enviroment.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "GameSystem.h"
#include <Component/Game.h>
#include <Component/Scripts.h>
#include <Component/Audio.h>
#include <EditorUI/ImGuiUI.h>
#include <GameStateManager/GameStateManager.h>

#ifndef IMGUI_DISABLE
#include <EditorUI/ImGuiUI.h>
#endif // !IMGUI_DISABLE


using namespace GE::Component;
using namespace GE::ECS;
using namespace GE::MONO;

void GE::Systems::GameSystem::Start()
{
  System::Start();
  GE::Events::EventManager::GetInstance().Subscribe<GE::Events::WindowLoseFocusEvent>(this);
  GE::Events::EventManager::GetInstance().Subscribe<GE::Events::GameNextTurn>(this);
  GE::Events::EventManager::GetInstance().Subscribe<GE::Events::GameTurnResolved>(this);
  m_shouldIterate = false;
}

void GE::Systems::GameSystem::Update()
{
  // There should only be 1 entity
  if (!m_entities.empty())
  {
    Game* game = m_ecs->GetComponent<Game>(*m_entities.begin());
    if (game == nullptr)
    {
      return;
    }

    if (m_lastShouldPause != m_shouldPause)
    {
      m_ecs->SetIsActiveEntity(game->m_pauseMenu, m_shouldPause);
      m_lastShouldPause = m_shouldPause;
    }

    /*if (m_shouldIterate)
    {*/
    static GE::FPS::FrameRateController& frc { GE::FPS::FrameRateController::GetInstance() };

    static GE::Debug::ErrorLogger& er = GE::Debug::ErrorLogger::GetInstance();
    bool hasError{ false };
    if (game->m_gameSystemScript.m_scriptClass == nullptr)
    {
      hasError = true;
      er.LogError("Game component does not have a game script");
      return;
    }

    MonoMethod* onUpdateFunc = mono_class_get_method_from_name(game->m_gameSystemScript.m_scriptClass, "OnUpdate", 1);
    double dt = frc.GetDeltaTime();
    std::vector<void*> params = { &dt };
    mono_runtime_invoke(onUpdateFunc, mono_gchandle_get_target(game->m_gameSystemScript.m_gcHandle), params.data(), nullptr);
      //MonoMethod* onUpdateFunc = mono_class_get_method_from_name(game->m_gameSystemScript.m_scriptClass, "OnUpdate", 8);
      /*Scripts* playerScript = m_ecs->GetComponent<Scripts>(game->m_player);
      Scripts* enemyScript = m_ecs->GetComponent<Scripts>(game->m_enemy);
      GE::MONO::ScriptInstance *playerStats, *enemyStats;
      playerStats = playerScript->Get("Stats");
      enemyStats = enemyScript->Get("Stats");

      if (onUpdateFunc == nullptr)
      {
        hasError = true;
        er.LogError("Game script does not have a function \"OnUpdate\" that takes in 8 parameters");
      }
      if (playerStats == nullptr)
      {
        hasError = true;
        er.LogError("Player stats script does not exist");
      }
      if (enemyStats == nullptr)
      {
        hasError = true;
        er.LogError("Enemy stats script does not exist");
      }

      if (hasError)
      {
        return;
      }
      else
      {
        double dt = frc.GetDeltaTime();
        void* args[] = { &dt, playerStats->m_classInst, &game->m_player, enemyStats->m_classInst, &game->m_enemy, &game->m_playerHand, &game->m_playerQueue, &game->m_enemyQueue };
        mono_runtime_invoke(onUpdateFunc, mono_gchandle_get_target(game->m_gameSystemScript.m_gcHandle), args, nullptr);
      }*/
    //}

    if (m_shouldWin)
    {
      m_shouldWin = false;
      //GE::GSM::GameStateManager::GetInstance().SetNextScene("Victory");
    }
    else if (m_shouldLose)
    {
      m_shouldLose = false;
      //GE::GSM::GameStateManager::GetInstance().SetNextScene("Defeat");
    }
  }
}

void GE::Systems::GameSystem::HandleEvent(GE::Events::Event* event)
{
  // There should only be 1 entity
  if (!m_entities.empty())
  {
    Game* game = m_ecs->GetComponent<Game>(*m_entities.begin());
    if (game == nullptr)
    {
      return;
    }

    switch (event->GetCategory())
    {
      case GE::Events::EVENT_TYPE::WINDOW_LOSE_FOCUS:
      {
        m_shouldPause = true;
        break;
      }
      case GE::Events::EVENT_TYPE::KEY_TRIGGERED:
      {
        KEY_CODE code = dynamic_cast<Events::KeyTriggeredEvent*>(event)->GetKey();
        switch (code)
        {
        case GPK_ESCAPE:
          // If lose or win, cannot pause
          if (m_shouldLose || m_shouldWin)
          {
            break;
          }
            m_shouldPause = !m_shouldPause;
          break;
//        case GPK_1:
//#ifndef IMGUI_DISABLE
//          if (GE::EditorGUI::ImGuiHelper::IsRunning())
//#endif // !IMGUI_DISABLE
//            GE::GSM::GameStateManager::GetInstance().SetNextScene("Victory");
//          //m_shouldWin = true;
//          break;
//        case GPK_2:
//#ifndef IMGUI_DISABLE
//          if (GE::EditorGUI::ImGuiHelper::IsRunning())
//#endif // !IMGUI_DISABLE
//          //m_shouldLose = true;
//            GE::GSM::GameStateManager::GetInstance().SetNextScene("Defeat");
//          break;
        default:
          break;
        }
        
        break;
      }
      case GE::Events::EVENT_TYPE::NEXT_TURN:
      {
        m_shouldIterate = true;
        break;
      }
      case GE::Events::EVENT_TYPE::TURN_RESOLVED:
      {
        m_shouldIterate = false;
        break;
      }
    }
  }
}

void GE::Systems::GameSystem::FlipPauseBool()
{
  m_shouldPause = !m_shouldPause;
}
