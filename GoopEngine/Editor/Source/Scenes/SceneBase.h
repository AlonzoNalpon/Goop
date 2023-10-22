#pragma once
#include <ObjectFactory/ObjectFactory.h>

namespace GE::Scenes
{
  class ISceneBase
  {
  public:
    virtual ~ISceneBase() {};
    virtual void Load() = 0; // load json
    virtual void Init() = 0; // creates all entities

    // No update or render as its part of ecs

    virtual void Unload() = 0; // kills all entities
    virtual void Free() = 0; // free sounds / images that were loaded from json
  };
}
