#include "pch.h"
#include "SceneManager.h"

using namespace GE::Scenes;

void SceneManager::Init()
{
  // Load data into map
  Scenes.emplace("Start", std::make_unique<GE::Scenes::SceneStart>());
  Scenes.emplace("SceneTest", std::make_unique<GE::Scenes::SceneTest>());

  // Testing
  CurrentScene = "SceneTest";
}

void SceneManager::LoadScene()
{
  Scenes[CurrentScene]->Load();
}

void SceneManager::InitScene()
{
  Scenes[CurrentScene]->Init();
}

void SceneManager::SetNextScene(std::string nextScene)
{
  // Make sure nextScene is a valid scene string then set it.
  CurrentScene = nextScene;
  LoadScene();
}
