#pragma once
#include "../Scenes/SceneBase.h"
#include <Scenes/SceneStart.h>
#include <Scenes/SceneTest.h>

namespace GE::Scenes
{
  class SceneManager
  {
  public:
    void  Init();
    void  LoadScene();
    void  InitScene();
    void  SetNextScene(std::string nextScene);

  private:
    std::string CurrentScene;
    std::map<std::string, std::unique_ptr<ISceneBase>>Scenes;
  };
}