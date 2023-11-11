#include <pch.h>
#include "GameSystem.h"

void GE::Systems::GameSystem::Update()
{
  for (GE::ECS::Entity entity : GetUpdatableEntities())
  {
    // Pass player and enemy entity stats to 
    // core game script
  }
}
