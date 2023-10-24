#pragma once
#include "SceneBase.h"

namespace GE::Scenes
{
  class SceneStart : public GE::Scenes::ISceneBase
  {
  public:
    void Load() override;
    void Load(std::string scene_name);
    void Init() override;
    void Unload() override;
    void Free() override;
  private:
    GE::ECS::EntityComponentSystem* ecs;
    GE::ObjectFactory::ObjectFactory* of;
    std::set<GE::ECS::Entity> m_entities;
  };
}