#pragma once
#include <string>
#include <ECS/EntityComponentSystem.h>
#include <ObjectFactory/ObjectFactory.h>

namespace GE::Scenes
{
  class Scene
  {
  public:
    virtual ~Scene() {};
    virtual void Load(std::string sceneName); // load json
    virtual void Init(); // creates all entities

    // No update or render as its part of ecs

    virtual void Unload(); // kills all entities
    virtual void Free(); // free sounds / images that were loaded from json
    void TestScene();
  private:
    GE::ECS::EntityComponentSystem* ecs;
    GE::ObjectFactory::ObjectFactory* of;
  };
}
