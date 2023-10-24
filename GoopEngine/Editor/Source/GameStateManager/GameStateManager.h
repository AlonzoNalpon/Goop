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
    void SetNextScene(std::string next_scene);
    void Restart();

    void Init();
    void Update();
    void Exit();
  };
}