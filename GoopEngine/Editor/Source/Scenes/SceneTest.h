#pragma once
#include "Scene.h"

namespace GE::Scenes
{
  class SceneTest : public GE::Scenes::Scene
  {
  public:
    void MakeDraggableBox();
    void Load(std::string sceneName) override; // load json & create entity
    void Init() override; // move all entity to position
    void Unload() override; // kills all entities
    void Free() override; // free sounds / images that were loaded from json
  private:
    GE::ECS::EntityComponentSystem* ecs;
    GE::ObjectFactory::ObjectFactory* of;
    std::set<GE::ECS::Entity> m_entities;
  };
}