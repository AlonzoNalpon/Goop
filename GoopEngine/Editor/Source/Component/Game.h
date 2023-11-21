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

    Game(std::string scriptName, std::vector<void*> arg, GE::ECS::Entity p, GE::ECS::Entity e)
    {
      player = p;
      enemy = e;
      m_gameSystemScript = GE::MONO::ScriptInstance(scriptName, arg);
    }
  };
}
