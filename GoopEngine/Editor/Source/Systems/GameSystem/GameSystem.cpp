#include <pch.h>
#include "GameSystem.h"
#include <Component/Game.h>
#include <Component/ScriptHandler.h>

using namespace GE::Component;
using namespace GE::ECS;

void GE::Systems::GameSystem::Update()
{
  for (Entity entity : GetUpdatableEntities())
  {
    Game* game = m_ecs->GetComponent<Game>(entity);
    Entity player = game->player;
    Entity enemy = game->enemy;
    // Pass player and enemy entity stats to 
    // core game script

    ScriptHandler* sh = m_ecs->GetComponent<GE::Component::ScriptHandler>(entity);
    // Invoke mono method passing in player and enemy data

  }
}
