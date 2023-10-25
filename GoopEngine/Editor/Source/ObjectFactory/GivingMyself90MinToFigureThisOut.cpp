#include <pch.h>
#include "GivingMyself90MinToFigureThisOut.h"
#include <ObjectFactory/ObjectFactory.h>

rttr::instance GE::ObjectFactory::GetEntityComponent(ECS::Entity id, ECS::COMPONENT_TYPES type)
{
  switch (type)
  {
  case ECS::COMPONENT_TYPES::TRANSFORM:
    return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Transform>(id);
  case ECS::COMPONENT_TYPES::BOX_COLLIDER:
    return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::BoxCollider>(id);
  case ECS::COMPONENT_TYPES::MODEL:
    return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Model>(id);
  case ECS::COMPONENT_TYPES::SCRIPT_HANDLER:
    return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::ScriptHandler>(id);
  case ECS::COMPONENT_TYPES::SPRITE:
    return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Sprite>(id);
  case ECS::COMPONENT_TYPES::SPRITE_ANIM:
    return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::SpriteAnim>(id);
  case ECS::COMPONENT_TYPES::TWEEN:
    return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Tween>(id);
  case ECS::COMPONENT_TYPES::VELOCITY:
    return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Velocity>(id);
  }

  return rttr::instance();
}
