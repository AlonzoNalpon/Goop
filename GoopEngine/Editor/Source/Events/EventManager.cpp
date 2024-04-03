/*!*********************************************************************
\file   EventManager.cpp
\author chengen.lau\@digipen.edu
\date   27-September-2023
\brief  
  
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "EventManager.h"
#include "InputEvents.h"
#include <Events/Events.h>
#include <Systems/Button/ButtonSystem.h>
#include <Systems/GameSystem/GameSystem.h>
#ifndef IMGUI_DISABLE
#include <EditorUI/EditorViewport.h>
#include <Prefabs/PrefabManager.h>
#include <EditorUI/PrefabEditor.h>
#endif

using namespace GE::Events;

void EventManager::SubscribeAllListeners()
{
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };

  Subscribe<MouseTriggeredEvent>(ecs.GetSystem<GE::Systems::ButtonSystem>());

#ifndef IMGUI_DISABLE
  EditorGUI::EditorViewport& evp{ EditorGUI::EditorViewport::GetInstance() };
  Prefabs::PrefabManager& pm{ Prefabs::PrefabManager::GetInstance() };
  EditorGUI::PrefabEditor& pe{ EditorGUI::PrefabEditor::GetInstance() };
  Assets::AssetManager& am{ Assets::AssetManager::GetInstance() };

  Subscribe<KeyTriggeredEvent>(&evp);

  Subscribe<UnloadSceneEvent>(&pm);
  Subscribe<RemoveEntityEvent>(&pm); Subscribe<DeletePrefabEvent>(&pm);

  Subscribe<EditPrefabEvent>(&pe); Subscribe<KeyTriggeredEvent>(&pe);
  Subscribe<DeletePrefabChildEvent>(&pe);

  Subscribe<NewSceneEvent>(&am); Subscribe<DeleteAssetEvent>(&am);
#endif
}
