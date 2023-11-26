#include <pch.h>
#include "GameSystem.h"
#include <Component/Game.h>
#include <Component/Scripts.h>
#include <Component/Audio.h>
#include <EditorUI/ImGuiUI.h>
#include <GameStateManager/GameStateManager.h>

#ifndef NO_IMGUI
#include <EditorUI/ImGuiUI.h>
#endif // !NO_IMGUI


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

    if (m_shouldIterate)
    {
      static GE::FPS::FrameRateController& frc { GE::FPS::FrameRateController::GetInstance() };

      static GE::Debug::ErrorLogger& er = GE::Debug::ErrorLogger::GetInstance();
      bool hasError{ false };
      if (game->m_gameSystemScript.m_scriptClass == nullptr)
      {
        hasError = true;
        er.LogError("Game component does not have a game script");
        return;
      }

      MonoMethod* onUpdateFunc = mono_class_get_method_from_name(game->m_gameSystemScript.m_scriptClass, "OnUpdate", 8);
      Scripts* playerScript = m_ecs->GetComponent<Scripts>(game->m_player);
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
        mono_runtime_invoke(onUpdateFunc, game->m_gameSystemScript.m_classInst, args, nullptr);
      }
    }

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
        case GPK_1:
#ifndef NO_IMGUI
          if (GE::EditorGUI::ImGuiHelper::IsRunning())
#endif // !NO_IMGUI
            GE::GSM::GameStateManager::GetInstance().SetNextScene("Victory");
          //m_shouldWin = true;
          break;
        case GPK_2:
#ifndef NO_IMGUI
          if (GE::EditorGUI::ImGuiHelper::IsRunning())
#endif // !NO_IMGUI
          //m_shouldLose = true;
            GE::GSM::GameStateManager::GetInstance().SetNextScene("Defeat");
          break;
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
