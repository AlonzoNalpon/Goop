#pragma once
#include <ECS/Entity/Entity.h>
#include <ScriptEngine/ScriptInstance.h>

namespace GE::Component
{
  struct Game
  {
    GE::ECS::Entity m_player;
    GE::ECS::Entity m_enemy;

    GE::ECS::Entity m_pauseMenu;

    GE::ECS::Entity m_playerHand;
    GE::ECS::Entity m_playerQueue;
    GE::ECS::Entity m_enemyQueue;

    GE::MONO::ScriptInstance m_gameSystemScript;
  };
}
