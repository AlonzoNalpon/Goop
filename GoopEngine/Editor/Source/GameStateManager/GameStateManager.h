#pragma once
#include <Singleton/Singleton.h>
#include <SceneManager/SceneManager.h>

namespace GE::GSM
{
  class GameStateManager
  {
  private:
    GE::Scenes::SceneManager sm;
    std::string currScene;
    std::string nextScene;

  public:
    GameStateManager();
    GameStateManager(std::string current_scene, std::string next_scene) :
      currScene{ current_scene },
      nextScene{ next_scene } {};
    void Init();

    void Update();

    void Exit();
  };
}