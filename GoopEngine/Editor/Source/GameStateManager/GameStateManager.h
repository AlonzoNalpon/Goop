#pragma once
#include <Singleton/Singleton.h>
#include <SceneManager/SceneManager.h>

namespace GE::GSM
{
  class GameStateManager
  {
  private:
    GE::Scenes::SceneManager sm;

  public:
    GameStateManager();

    void SetNextScene(std::string next_scene);

    void Init();

    void Update();

    void Exit();
  };
}