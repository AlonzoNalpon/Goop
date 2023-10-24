#include "pch.h"
#include "SceneManager.h"

using namespace GE::Scenes;

void SceneManager::Init()
{
  // Load data into map
  Scenes.emplace("Start", std::make_unique<GE::Scenes::SceneStart>());
  Scenes.emplace("SceneTest", std::make_unique<GE::Scenes::SceneTest>());

  // Testing
  m_nextScene = m_currentScene = "Start";
}

void SceneManager::LoadScene()
{
  Scenes[m_currentScene]->Load();
}

void SceneManager::InitScene()
{
  Scenes[m_currentScene]->Init();
}

void SceneManager::UnloadScene()
{
  Scenes[m_currentScene]->Unload();
}

void SceneManager::FreeScene()
{
  Scenes[m_currentScene]->Free();
}

void SceneManager::SetNextScene(std::string nextScene)
{
  // Make sure nextScene is a valid scene string then set it.
  if (Scenes.find(nextScene) == Scenes.end())
  {
    throw GE::Debug::Exception<SceneManager>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to load scene: " + nextScene));
  }
  this->UnloadScene();
  this->FreeScene();
  Scenes[m_currentScene]->Free();
  m_nextScene = m_currentScene = nextScene;
  LoadScene(); 
  InitScene();
}

void GE::Scenes::SceneManager::RestartScene()
{
  Scenes[m_currentScene]->Unload();
  Scenes[m_currentScene]->Init();
}

std::string SceneManager::GetCurrentScene()
{
  return this->m_currentScene;
}

std::string GE::Scenes::SceneManager::GetNextScene()
{
  return this->m_nextScene;
}
