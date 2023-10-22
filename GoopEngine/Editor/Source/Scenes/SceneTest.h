#pragma once
#include "SceneBase.h"

namespace GE::Scenes
{
  class SceneTest : public GE::Scenes::ISceneBase
  {
  public:
    //SceneTest();
    void MakeDraggableBox();
    void Load() override; // load json
    void Init() override; // creates all entities
    void Unload() override; // kills all entities
    void Free() override; // free sounds / images that were loaded from json
  private:
    GE::ECS::EntityComponentSystem* ecs;
    GE::ObjectFactory::ObjectFactory* of;
  };
}