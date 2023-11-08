#pragma once
#include <vector>
#include <Math/GEM.h>
#include <ECS/Entity/Entity.h>

namespace GE::Component
{
  struct
  {
    std::vector<GE::ECS::Entity> grids;
  };
}
