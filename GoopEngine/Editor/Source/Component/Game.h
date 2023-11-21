#pragma once
#include <ECS/Entity/Entity.h>
#include <ScriptEngine/ScriptInstance.h>

namespace GE::Component
{
  struct Game
  {
    GE::ECS::Entity player;
    GE::ECS::Entity enemy;

    //GE::MONO::ScriptInstance playerScript;
    //GE::MONO::ScriptInstance enemyScript;

    GE::MONO::ScriptInstance m_gameSystemScript;
  };
}
