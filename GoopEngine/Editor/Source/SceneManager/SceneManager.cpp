/*!*********************************************************************
\file   SceneManager.h
\author loh.j@digipen.edu
\date   24-October-2023
\brief
  In charge of:
    - Initializing,
    - Loading,
    - Unloading,
    - Freeing
  scenes to display.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include "pch.h"
#include "SceneManager.h"
#include <Serialization/Serializer.h>
#include <AssetManager/AssetManager.h>
#include <Events/EventManager.h>
#ifndef IMGUI_DISABLE
#include <Prefabs/PrefabManager.h>
#include <filesystem>
#include <EditorUI/ImGuiUI.h>
#endif

using namespace GE::Scenes;

SceneManager::SceneManager() : m_currentScene{ "Start" }, m_nextScene{ "Start" } {}

void SceneManager::Init()
{
#ifndef IMGUI_DISABLE
  std::string const tempDir{ am->GetConfigData<std::string>("TempDir") };
  m_tempPath = tempDir + ".tmpscn";
  // create temp directory if it doesn't already exist
  if (!std::filesystem::exists(tempDir) || !std::filesystem::is_directory(tempDir))
  {
    if (std::filesystem::create_directory(tempDir))
      Debug::ErrorLogger::GetInstance().LogMessage("Created temp directory at: " + tempDir);
    else
      Debug::ErrorLogger::GetInstance().LogError("Unable to create temp directory at: " + tempDir + ". Scene reloading features may be unavailable!");
  }

  // subscribe to scene events
  Events::EventManager& em{ Events::EventManager::GetInstance() };
  em.Subscribe<Events::StartSceneEvent>(this); em.Subscribe<Events::StopSceneEvent>(this);
  em.Subscribe<Events::EditPrefabEvent>(this, Events::PRIORITY::HIGH);
  em.Subscribe<Events::PrefabInstancesUpdatedEvent>(this);
  em.Subscribe<Events::NewSceneEvent>(this);
#endif

  // Load data into map
  m_nextScene = m_currentScene = GE::Assets::AssetManager::GetInstance().GetConfigData<std::string>("StartUpScn");
}

void SceneManager::LoadScene()
{
  scene.Load(GE::Assets::AssetManager::GetInstance().GetScene(m_currentScene));
}

void SceneManager::InitScene()
{
  scene.Init();

#ifdef IMGUI_DISABLE
  // when editor isn't running, invoke OnCreate after Init
  InvokeOnCreate();
#endif
}

void SceneManager::UnloadScene()
{
  scene.Unload();
  Events::EventManager::GetInstance().Dispatch(Events::UnloadSceneEvent());
}

void SceneManager::FreeScene()
{
  scene.Free();
}

void SceneManager::SetNextScene(std::string nextScene)
{
  std::string tmpScene {m_currentScene};
  UnloadScene();
  FreeScene();
  m_nextScene = m_currentScene = nextScene;
  
  try
  {
    LoadScene();
    InitScene();

#ifndef IMGUI_DISABLE
    // if running editor, we dont invoke OnCreate if the scene isnt running
    if (EditorGUI::ImGuiHelper::IsRunning())
#endif
    InvokeOnCreate();
  }
  catch (std::out_of_range&)
  {
    m_nextScene = m_currentScene = tmpScene;

    LoadScene();
    InitScene();

#ifndef IMGUI_DISABLE
    // if running editor, we dont invoke OnCreate if the scene isnt running
    if (EditorGUI::ImGuiHelper::IsRunning())
#endif
    InvokeOnCreate();
    throw Debug::Exception<SceneManager>(Debug::LEVEL_CRITICAL, ErrMsg(nextScene + ".scn doesn't exist."));
  }
}

void GE::Scenes::SceneManager::RestartScene()
{
  UnloadScene();
  InitScene();
}

void GE::Scenes::SceneManager::HandleEvent(Events::Event* event)
{
#ifndef IMGUI_DISABLE
  switch (event->GetCategory())
  {
  case Events::EVENT_TYPE::NEW_SCENE:
    UnloadScene();
    FreeScene();
    m_nextScene = m_currentScene = static_cast<Events::NewSceneEvent*>(event)->m_sceneName;
    break;

  case Events::EVENT_TYPE::START_SCENE:
    TemporarySave();  // temporarily save scene before play
    InvokeOnCreate(); // invoke OnCreate on Play button press
    break;

  case Events::EVENT_TYPE::STOP_SCENE:
    LoadTemporarySave();  // revert to previous state before play
    break;

  case Events::EVENT_TYPE::PREFAB_INSTANCES_UPDATED:
    if (m_tempSaves.empty())
    {
      SaveScene();
    }
    else
    {
      Serialization::Serializer::SerializeScene(m_tempSaves.top().m_path);
    }

    GE::Debug::ErrorLogger::GetInstance().LogMessage("Scene's prefab instances have been updated");
    break;

  case Events::EVENT_TYPE::EDIT_PREFAB:
  {
    // save and unload
    TemporarySave();
    UnloadScene();
    FreeScene();

    m_nextScene = m_currentScene = "Prefab Editor";
    break;
  }
  }
#endif
}

void GE::Scenes::SceneManager::LoadSceneFromExplorer(std::string const& filepath)
{
  std::string::size_type const startPos{ filepath.find_last_of("\\") + 1 };
  std::string const filename{ filepath.substr(startPos, filepath.find_last_of(".") - startPos) };
  // reload files if it doesn't exist
  Assets::AssetManager::GetInstance().ReloadFiles(Assets::SCENE);
  SetNextScene(filename);
}

std::string GE::Scenes::SceneManager::GetCurrentScene() const noexcept
{
  return this->m_currentScene;
}

std::string GE::Scenes::SceneManager::GetNextScene() const noexcept
{
  return this->m_nextScene;
}

void GE::Scenes::SceneManager::SaveScene() const
{
  // Save systems back to original file
  //Serialization::SerializeSystems(*Assets::AssetManager::GetInstance().GetConfigData<std::string>("Systems"));
 
  // Save the scene
  std::ostringstream filepath{};
  filepath << am->GetConfigData<std::string>("Assets Dir")
    << "Scenes/" << m_currentScene << am->GetConfigData<std::string>("Scene File Extension");
  Serialization::Serializer::SerializeScene(filepath.str());

  GE::Debug::ErrorLogger::GetInstance().LogMessage("Successfully saved scene to " + filepath.str());
}

void GE::Scenes::SceneManager::InvokeOnCreate() const
{
  ECS::EntityComponentSystem& ecs = { ECS::EntityComponentSystem::GetInstance() };

  // invoke all scripts' OnCreate() function
  for (auto const& e : ecs.GetEntities())
  {
    if (ecs.GetIsActiveEntity(e))
    {
      if (ecs.HasComponent<GE::Component::Scripts>(e))
      {
        GE::Component::Scripts* scripts = ecs.GetComponent<GE::Component::Scripts>(e);
        for (auto script : scripts->m_scriptList)
        {
          script.InvokeOnCreate();
        }
      }
      else if (ecs.HasComponent<GE::Component::Game>(e))
      {
        GE::Component::Game* game = ecs.GetComponent<GE::Component::Game>(e);
        game->m_gameSystemScript.InvokeOnCreate();
      }
    }
  }
}

#ifndef IMGUI_DISABLE
void GE::Scenes::SceneManager::TemporarySave()
{
  std::string const path{ m_tempPath + std::to_string(m_tempSaves.size()) };
  m_tempSaves.emplace(m_currentScene, path);
  Serialization::Serializer::SerializeScene(path);

  GE::Debug::ErrorLogger::GetInstance().LogMessage("Scene has been temporarily saved");
}

void GE::Scenes::SceneManager::LoadTemporarySave()
{
  UnloadScene();
  FreeScene();

  m_nextScene = m_currentScene = m_tempSaves.top().m_name;
  GE::Debug::ErrorLogger::GetInstance().LogMessage("Reverting scene's previous state... (" + m_currentScene + ")");
  scene.Load(m_tempSaves.top().m_path);
  std::remove(m_tempSaves.top().m_path.c_str()); // delete temp scene file
  m_tempSaves.pop();
  InitScene();
}
#endif
