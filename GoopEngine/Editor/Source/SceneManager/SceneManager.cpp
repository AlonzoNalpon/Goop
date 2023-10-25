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
  std::string tmpScene {m_currentScene};
  UnloadScene();
  FreeScene();
  m_nextScene = m_currentScene = nextScene;
  
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
