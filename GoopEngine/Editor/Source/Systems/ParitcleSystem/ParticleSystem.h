#pragma once
#include <ECS/System/System.h>

namespace GE::Systems
{
  class ParticleSystem : public GE::ECS::System
  {
    void Update();
  };
}
