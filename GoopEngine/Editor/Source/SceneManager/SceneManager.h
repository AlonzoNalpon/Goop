#pragma once
#include "../Scenes/SceneBase.h"
#include <Scenes/SceneStart.h>
#include <Scenes/SceneTest.h>

namespace GE::Scenes
{
  class SceneManager
  {
  public:
    SceneManager() : m_currentScene{ "Start" }, m_nextScene{ "Start" }, Scenes{} {};
    SceneManager(std::string current_scene, std::string next_scene) :
      m_currentScene{ current_scene },
      m_nextScene{ next_scene },
      Scenes{} {};
    void Init();
    void LoadScene();
    void InitScene();
    void UnloadScene();
    void FreeScene();
    void SetNextScene(std::string nextScene);
    std::string GetCurrentScene();
    std::string GetNextScene();

  private:
    std::string m_currentScene;
    std::string m_nextScene;
    std::map<std::string, std::unique_ptr<ISceneBase>>Scenes;
  };
}