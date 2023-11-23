#include <pch.h>
#include "GameSystem.h"
#include <Component/Game.h>
#include <Component/Scripts.h>
#include <Component/Audio.h>

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

    if (m_shouldPause)
    {
      m_ecs->SetIsActiveEntity(game->m_pauseMenu, true);
      m_shouldPause = false;
    }

    if (m_shouldIterate)
    {
      static GE::FPS::FrameRateController& frc { GE::FPS::FrameRateController::GetInstance() };
      MonoMethod* onUpdateFunc = mono_class_get_method_from_name(game->m_gameSystemScript.m_scriptClass, "OnUpdate", 5);
      Scripts* playerScript = m_ecs->GetComponent<Scripts>(game->m_player);
      Scripts* enemyScript = m_ecs->GetComponent<Scripts>(game->m_enemy);
      GE::MONO::ScriptInstance it, it2;
      for (auto& [name, script] : playerScript->m_scriptList)
      {
        if (name == "Stats")
        {
          it = script;
        }
      }
      for (auto& [name, script] : enemyScript->m_scriptList)
      {
        if (name == "Stats")
        {
          it = script;
        }
      }
      double dt = frc.GetDeltaTime();
      void* args[] = { &dt, &it.m_classInst, &game->m_player, &it2.m_classInst, &game->m_enemy };
      //void* args[] = { &dt, it->second.m_classInst, it2->second.m_classInst };
      mono_runtime_invoke(onUpdateFunc, game->m_gameSystemScript.m_classInst, args, nullptr);
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