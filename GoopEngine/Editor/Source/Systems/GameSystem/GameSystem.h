#pragma once
#include <ECS/System/System.h>

namespace GE::Systems
{
  class GameSystem : public GE::ECS::System
  {
  public:
    void Update();
  };
}
