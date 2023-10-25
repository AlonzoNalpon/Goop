#include "pch.h"
#include "SceneManager.h"

using namespace GE::Scenes;

void SceneManager::Init()
{
  // Load data into map
  m_nextScene = m_currentScene = "Start";
}

void SceneManager::LoadScene()
{
  scene.Load(m_currentScene);
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
  // Make sure nextScene is a valid scene string then set it.
  /*if (am->GetMapData("m_scenes").find(nextScene) == am->GetMapData("m_scenes").end())
  {
    throw GE::Debug::Exception<SceneManager>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to load scene: " + nextScene));
  }*/
  if (nextScene == "SceneTest")
  {
    for (auto& i : am->GetMapData("m_scenes"))
    {
      std::cout << i.second << std::endl;
    }
    this->UnloadScene();
    this->FreeScene();
    m_nextScene = m_currentScene = nextScene;
    scene.TestScene();
  }
  if (nextScene != "SceneTest")
  {
    UnloadScene();
    FreeScene();
    m_nextScene = m_currentScene = nextScene;
    LoadScene();
    InitScene();
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
  Init();
  LoadScene();
  InitScene();
}

std::string SceneManager::GetCurrentScene()
{
  return this->m_currentScene;
}

std::string GE::Scenes::SceneManager::GetNextScene()
{
  return this->m_nextScene;
}
