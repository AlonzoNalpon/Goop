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

    //GE::MONO::ScriptInstance playerScript;
    //GE::MONO::ScriptInstance enemyScript;

    GE::MONO::ScriptInstance m_gameSystemScript;
  };
}
