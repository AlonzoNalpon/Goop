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

using namespace GE::Scenes;

SceneManager::SceneManager() : m_currentScene{ "Start" }, m_nextScene{ "Start" } {}

void SceneManager::Init()
{
  m_tempScene = am->GetConfigData<std::string>("TempDir") + ".tmpscn";
  // subscribe to scene events
  Events::EventManager& em{ Events::EventManager::GetInstance() };
  em.Subscribe<Events::StartSceneEvent>(this); em.Subscribe<Events::PauseSceneEvent>(this); em.Subscribe<Events::StopSceneEvent>(this);

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
}

void SceneManager::UnloadScene()
{
  scene.Unload();
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
  m_nextScene = m_currentScene = std::move(nextScene);
  
  try
  {
    if (nextScene == "SceneTest")
    {
      scene.TestScene();
    }
    else
    {
      LoadScene();
      InitScene();
    }
  }
  catch (std::out_of_range&)
  {
    m_nextScene = m_currentScene = tmpScene;
    if (m_currentScene == "SceneTest")
    {
      scene.TestScene();
    }
    else
    {
      LoadScene();
      InitScene();
    }
    throw Debug::Exception<SceneManager>(Debug::LEVEL_CRITICAL, ErrMsg(nextScene + ".scn doesn't exist."));
  }
}

void GE::Scenes::SceneManager::RestartScene()
{
  if (m_currentScene == "SceneTest")
  {
    UnloadScene();
    scene.TestScene();
    return;
  }
  UnloadScene();
  InitScene();
}

void GE::Scenes::SceneManager::HandleEvent(Events::Event* event)
{
  switch (event->GetCategory())
  {
  case Events::EVENT_TYPE::START_SCENE:
    TemporarySave();  // temporarily save scene before play
    break;
  case Events::EVENT_TYPE::STOP_SCENE:
    LoadTemporarySave();  // revert to previous state before play
    break;
  }
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

void GE::Scenes::SceneManager::TemporarySave() const
{
  std::string const filepath{ m_tempScene };
  Serialization::Serializer::SerializeScene(filepath);

  GE::Debug::ErrorLogger::GetInstance().LogMessage("Scene has been temporarily saved");
}

void GE::Scenes::SceneManager::LoadTemporarySave()
{
  UnloadScene();
  FreeScene();

  GE::Debug::ErrorLogger::GetInstance().LogMessage("Reverting scene's previous state...");
  scene.Load(m_tempScene);
  std::remove(m_tempScene.c_str()); // delete temp scene file
}
