#pragma once
#include <ECS/Entity/Entity.h>

namespace GE::Component
{
  struct Game
  {
    GE::ECS::Entity player;
    GE::ECS::Entity enemy;
  };
}
