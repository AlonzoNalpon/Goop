#pragma once
#include <ObjectFactory/ObjectFactory.h>

#include <Component/Velocity.h>
#include <Component/Transform.h>
#include <Component/Sprite.h>
#include <Component/SpriteAnim.h>
#include <Component/BoxCollider.h>
#include <Component/Tween.h>
#include <Component/Model.h>
#include <Component/ScriptHandler.h>

#include <Systems/Physics/PhysicsSystem.h>
#include <Systems/Physics/CollisionSystem.h>
#include <Systems/DraggableObject/DraggableObjectSystem.h>
#include <Systems/PlayerController/PlayerControllerSystem.h>
#include <Systems/RootTransform/RootTransformSystem.h>
#include <Systems/Rendering/RenderingSystem.h>
#include <Systems/SpriteAnim/SpriteAnimSystem.h>

#include <Audio/AudioEngine.h>

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
  private:

  };
}
