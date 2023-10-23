#pragma once
#include "SceneBase.h"

namespace GE::Scenes
{
  class SceneStart : public GE::Scenes::ISceneBase
  {
  public:
    void Load() override;
    void Init() override;
    void Unload() override;
    void Free() override;
  private:
  };
}