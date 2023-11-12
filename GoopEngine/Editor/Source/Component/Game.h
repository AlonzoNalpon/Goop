#pragma once
#include <ECS/Entity/Entity.h>
#include <ScriptEngine/Script.h>

namespace GE::Component
{
  struct Game
  {
    GE::ECS::Entity player;
    GE::ECS::Entity enemy;

    GE::MONO::Script playerScript;
    GE::MONO::Script enemyScript;

    GE::MONO::Script gameSystemScript;
  };
}
