/*!*********************************************************************
\file   EventManager.cpp
\author chengen.lau\@digipen.edu
\date   27-September-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "../Systems/DraggableObject/DraggableObjectSystem.h"
#include "EventManager.h"
#include "InputEvents.h"

using namespace GE::Events;

void EventManager::SubscribeAllListeners()
{
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };

  Subscribe<MouseHeldEvent>(ecs.GetSystem<GE::Systems::DraggableObjectSystem>());
  Subscribe<MouseReleasedEvent>(ecs.GetSystem<GE::Systems::DraggableObjectSystem>());
}
