/*!*********************************************************************
\file   ObjectFactory.cpp
\author loh.j@digipen.edu
\date   28 September 2023
\brief
  Contains the definition of the ObjectFactory singleton class, which
  encapsulates functions and data members to facilitate the creation
  of objects in the scene. The class serves as a middle-man between
  serialization and the actual entities in the scene. It is responsible
  for the creation/cloning of entities and adding of components through
  the ECS.

  On load, the ObjectFactory holds the deserialized data for the scene.
  Upon reloading, the objects are loaded from here without the need to
  deserialize again. Only when changing scenes will a full reload be
  required.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ObjectFactory.h"
#include <AssetManager/AssetManager.h>
#include <Systems/Systems.h>
#include "SerializeComponents.h"
#include <Systems/Rendering/RenderingSystem.h>
#include <Serialization/Deserializer.h>
#ifndef IMGUI_DISABLE
#include <Prefabs/PrefabManager.h>
#endif

using namespace GE::ObjectFactory;
using namespace GE::ECS;

void ObjectFactory::AddComponentsToEntity(ECS::Entity id, std::vector<rttr::variant> const& components) const
{
  for (rttr::variant const& component : components)
  {
    AddComponentToEntity(id, component);
  }
}

std::vector<rttr::variant> ObjectFactory::GetEntityComponents(ECS::Entity id) const
{
  std::vector<rttr::variant> ret;
  ret.reserve(ECS::EntityComponentSystem::GetInstance().GetEntityComponentCount(id));
  for (unsigned i{}; i < static_cast<unsigned>(ECS::COMPONENT_TYPES::COMPONENTS_TOTAL); ++i)
  {
    rttr::variant comp{ GetEntityComponent(id, static_cast<ECS::COMPONENT_TYPES>(i)) };
    if (!comp.is_valid()) { continue; }

    ret.emplace_back(std::move(comp));
  }

  return ret;
}

void GE::ObjectFactory::ObjectFactory::EmptyMap()
{
  m_deserialized.clear();
}

void ObjectFactory::CloneObject(ECS::Entity entity, ECS::Entity parent) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  Entity newEntity = ecs.CreateEntity();
  ecs.SetEntityName(newEntity, ecs.GetEntityName(entity) + " (Copy)");
  ecs.SetIsActiveEntity(newEntity, ecs.GetIsActiveEntity(entity));

  std::vector<rttr::variant> const components{ GetEntityComponents(entity) };
  AddComponentsToEntity(entity, components);

#ifndef IMGUI_DISABLE
  // update entity's prefab if needed
  Prefabs::PrefabManager& pm{ Prefabs::PrefabManager::GetInstance() };
  auto const entityPrefab{ pm.GetEntityPrefab(entity) };
  if (entityPrefab)
  {
    pm.AttachPrefab(newEntity, *entityPrefab);
  }
#endif

  // set parent/child
  ecs.SetParentEntity(newEntity, parent);
  if (parent != ECS::INVALID_ID) { ecs.AddChildEntity(parent, newEntity); }
  for (ECS::Entity const& child : ecs.GetChildEntities(entity))
  {
    CloneObject(child, newEntity);  // recursively clone all children
  }
}

void ObjectFactory::ClearSceneObjects()
{
  m_deserialized.clear();
}

void ObjectFactory::LoadSceneObjects()
{
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };

  for (auto const& [id, data] : m_deserialized)
  {
    ecs.CreateEntity({}, id, data.m_name);
    ecs.SetIsActiveEntity(const_cast<ECS::Entity&>(id), data.m_isActive);
    AddComponentsToEntity(id, data.m_components);
  }

  for (auto const& [id, data] : m_deserialized)
  {
    ecs.SetParentEntity(id, data.m_parent);

    for (ECS::Entity const& child : data.m_childEntities)
    {
      ecs.AddChildEntity(id, child);
    }
  }

#ifndef IMGUI_DISABLE
  if (Prefabs::PrefabManager::GetInstance().UpdateAllEntitiesFromPrefab())
  {
    Events::EventManager::GetInstance().Dispatch(Events::PrefabInstancesUpdatedEvent());
  }
#endif
}

void ObjectFactory::LoadSceneJson(std::string const& filename)
{
  m_deserialized = GE::Serialization::Deserializer::DeserializeScene(filename);
}

void ObjectFactory::AddComponentToEntity(ECS::Entity entity, rttr::variant const& compVar) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };
  rttr::type compType{ compVar.get_type() };
  // get underlying type if it's wrapped in a pointer
  compType = compType.is_wrapper() ? compType.get_wrapped_type().get_raw_type() : compType.is_pointer() ? compType.get_raw_type() : compType;

  if (compType == rttr::type::get<Component::Transform>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Transform*>());
  }
  else if (compType == rttr::type::get<Component::BoxCollider>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::BoxCollider*>());
  }
  else if (compType == rttr::type::get<Component::Velocity>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Velocity*>());
  }
  else if (compType == rttr::type::get<Component::Sprite>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Sprite*>());
  }
  else if (compType == rttr::type::get<Component::SpriteAnim>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::SpriteAnim*>());
  }
  else if (compType == rttr::type::get<Component::Model>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Model*>());
  }
  else if (compType == rttr::type::get<Component::Tween>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Tween*>());
  }
  else if (compType == rttr::type::get<Component::Scripts>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Scripts*>());
  }
  else if (compType == rttr::type::get<Component::Draggable>())
  {
    ecs.AddComponent(entity, Component::Draggable{});
  }
  else if (compType == rttr::type::get<Component::EnemyAI>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::EnemyAI*>());
  }
  else if (compType == rttr::type::get<Component::Text>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Text*>());
  }
  else if (compType == rttr::type::get<Component::Audio>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Audio*>());
  }
  else if (compType == rttr::type::get<Component::GE_Button>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::GE_Button*>());
  }
  else if (compType == rttr::type::get<Component::Anchor>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Anchor*>());
  }
  else if (compType == rttr::type::get<Component::Card>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Card*>());
  }
  else if (compType == rttr::type::get<Component::Game>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::Game*>());
  }
  else if (compType == rttr::type::get<Component::CardHolder>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::CardHolder*>());
  }
  else if (compType == rttr::type::get<Component::CardHolderElem>())
  {
    ecs.AddComponent(entity, *compVar.get_value<Component::CardHolderElem*>());
  }
  else
  {
    std::ostringstream oss{};
    oss << "Trying to add unknown component type: " << compType.get_name().to_string() << " to entity " << entity << " | Update ObjectFactory::AddComponentToEntity";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
  }
}

rttr::variant ObjectFactory::GetEntityComponent(ECS::Entity id, ECS::COMPONENT_TYPES type) const
{
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
  switch (type)
  {
  case ECS::COMPONENT_TYPES::TRANSFORM:
  {
    return ecs.HasComponent<Component::Transform>(id) ? std::make_shared<Component::Transform>(*ecs.GetComponent<Component::Transform>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::BOX_COLLIDER:
  {
    return ecs.HasComponent<Component::BoxCollider>(id) ? std::make_shared<Component::BoxCollider>(*ecs.GetComponent<Component::BoxCollider>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::VELOCITY:
  {
    return ecs.HasComponent<Component::Velocity>(id) ? std::make_shared<Component::Velocity>(*ecs.GetComponent<Component::Velocity>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::SCRIPTS:
  {
    return ecs.HasComponent<Component::Scripts>(id) ? std::make_shared<Component::Scripts>(*ecs.GetComponent<Component::Scripts>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::SPRITE:
  {
    return ecs.HasComponent<Component::Sprite>(id) ? std::make_shared<Component::Sprite>(*ecs.GetComponent<Component::Sprite>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::SPRITE_ANIM:
  {
    return ecs.HasComponent<Component::SpriteAnim>(id) ? std::make_shared<Component::SpriteAnim>(*ecs.GetComponent<Component::SpriteAnim>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::TWEEN:
  {
    return ecs.HasComponent<Component::Tween>(id) ? std::make_shared<Component::Tween>(*ecs.GetComponent<Component::Tween>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::ENEMY_AI:
  {
    return ecs.HasComponent<Component::EnemyAI>(id) ? std::make_shared<Component::EnemyAI>(*ecs.GetComponent<Component::EnemyAI>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::DRAGGABLE:
  {
    return ecs.HasComponent<Component::Draggable>(id) ? std::make_shared<Component::Draggable>(*ecs.GetComponent<Component::Draggable>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::TEXT:
  {
    return ecs.HasComponent<Component::Text>(id) ? std::make_shared<Component::Text>(*ecs.GetComponent<Component::Text>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::AUDIO:
  {
    return ecs.HasComponent<Component::Audio>(id) ? std::make_shared<Component::Audio>(*ecs.GetComponent<Component::Audio>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::GE_BUTTON:
  {
    return ecs.HasComponent<Component::GE_Button>(id) ? std::make_shared<Component::GE_Button>(*ecs.GetComponent<Component::GE_Button>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::ANCHOR:
  {
    return ecs.HasComponent<Component::Anchor>(id) ? std::make_shared<Component::Anchor>(*ecs.GetComponent<Component::Anchor>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::CARD:
  {
    return ecs.HasComponent<Component::Card>(id) ? std::make_shared<Component::Card>(*ecs.GetComponent<Component::Card>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::CARD_HOLDER:
  {
    return ecs.HasComponent<Component::CardHolder>(id) ? std::make_shared<Component::CardHolder>(*ecs.GetComponent<Component::CardHolder>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::CARD_HOLDER_ELEM:
  {
    return ecs.HasComponent<Component::CardHolderElem>(id) ? std::make_shared<Component::CardHolderElem>(*ecs.GetComponent<Component::CardHolderElem>(id)) : rttr::variant();
  }
  case ECS::COMPONENT_TYPES::GAME:
  {
    return ecs.HasComponent<Component::Game>(id) ? std::make_shared<Component::Game>(*ecs.GetComponent<Component::Game>(id)) : rttr::variant();
  }
  default:
  {
    std::ostringstream oss{};
    std::string const enumString = rttr::type::get<ECS::COMPONENT_TYPES>().get_enumeration().value_to_name(type).to_string();
    oss << "Trying to get unsupported component type (" << enumString << ") from Entity " << id;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return rttr::variant();
  }
  }
}

void ObjectFactory::RemoveComponentFromEntity(ECS::Entity entity, rttr::type compType) const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };
  // get underlying type if it's wrapped in a pointer
  compType = compType.is_wrapper() ? compType.get_wrapped_type().get_raw_type() : compType.is_pointer() ? compType.get_raw_type() : compType;

  if (compType == rttr::type::get<Component::Transform>())
  {
    ecs.RemoveComponent<Transform>(entity);
  }
  else if (compType == rttr::type::get<Component::BoxCollider>())
  {
    ecs.RemoveComponent<BoxCollider>(entity);
  }
  else if (compType == rttr::type::get<Component::Velocity>())
  {
    ecs.RemoveComponent<Velocity>(entity);
  }
  else if (compType == rttr::type::get<Component::Sprite>())
  {
    ecs.RemoveComponent<Sprite>(entity);
  }
  else if (compType == rttr::type::get<Component::SpriteAnim>())
  {
    ecs.RemoveComponent<SpriteAnim>(entity);
  }
  else if (compType == rttr::type::get<Component::Model>())
  {
    ecs.RemoveComponent<Model>(entity);
  }
  else if (compType == rttr::type::get<Component::Tween>())
  {
    ecs.RemoveComponent<Tween>(entity);
  }
  else if (compType == rttr::type::get<Component::Scripts>())
  {
    ecs.RemoveComponent<Scripts>(entity);
  }
  else if (compType == rttr::type::get<Component::Draggable>())
  {
    ecs.RemoveComponent<Draggable>(entity);
  }
  else if (compType == rttr::type::get<Component::EnemyAI>())
  {
    ecs.RemoveComponent<EnemyAI>(entity);
  }
  else if (compType == rttr::type::get<Component::Text>())
  {
    ecs.RemoveComponent<Text>(entity);
  }
  else if (compType == rttr::type::get<Component::Audio>())
  {
    ecs.RemoveComponent<Audio>(entity);
  }
  else if (compType == rttr::type::get<Component::GE_Button>())
  {
    ecs.RemoveComponent<GE_Button>(entity);
  }
  else if (compType == rttr::type::get<Component::Anchor>())
  {
    ecs.RemoveComponent<Anchor>(entity);
  }
  else if (compType == rttr::type::get<Component::Card>())
  {
    ecs.RemoveComponent<Card>(entity);
  }
  else if (compType == rttr::type::get<Component::Game>())
  {
    ecs.RemoveComponent<Game>(entity);
  }
  else if (compType == rttr::type::get<Component::CardHolder>())
  {
    ecs.RemoveComponent<CardHolder>(entity);
  }
  else if (compType == rttr::type::get<Component::CardHolderElem>())
  {
    ecs.RemoveComponent<CardHolderElem>(entity);
  }
  else
  {
    std::ostringstream oss{};
    oss << "Trying to remove unknown component type: " << compType.get_name().to_string() << " to entity " << entity << " | Update ObjectFactory::RemoveComponentFromEntity";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
  }
}

void ObjectFactory::RegisterComponentsAndSystems() const
{
  EntityComponentSystem& ecs{ EntityComponentSystem::GetInstance() };

  // Register components in order of COMPONENT_TYPES enum
  for (auto const& elem : GE::ECS::componentsToString)
  {
    switch (elem.first)
    {
    case COMPONENT_TYPES::TRANSFORM:
      ecs.RegisterComponent<GE::Component::Transform>();
      break;
    case COMPONENT_TYPES::BOX_COLLIDER:
      ecs.RegisterComponent<GE::Component::BoxCollider>();
      break;
    case COMPONENT_TYPES::SPRITE_ANIM:
      ecs.RegisterComponent<GE::Component::SpriteAnim>();
      break;
    case COMPONENT_TYPES::SPRITE:
      ecs.RegisterComponent<GE::Component::Sprite>();
      break;
    case COMPONENT_TYPES::VELOCITY:
      ecs.RegisterComponent<GE::Component::Velocity>();
      break;
    case COMPONENT_TYPES::TWEEN:
      ecs.RegisterComponent<GE::Component::Tween>();
      break;
    case COMPONENT_TYPES::SCRIPTS:
      ecs.RegisterComponent<GE::Component::Scripts>();
      break;
    case COMPONENT_TYPES::DRAGGABLE:
      ecs.RegisterComponent<GE::Component::Draggable>();
      break;
    case COMPONENT_TYPES::ENEMY_AI:
      ecs.RegisterComponent<GE::Component::EnemyAI>();
      break;
    case COMPONENT_TYPES::TEXT:
      ecs.RegisterComponent<GE::Component::Text>();
      break;
    case COMPONENT_TYPES::GAME:
      ecs.RegisterComponent<GE::Component::Game>();
      break;
    case COMPONENT_TYPES::AUDIO:
      ecs.RegisterComponent<GE::Component::Audio>();
      break;
    case COMPONENT_TYPES::GE_BUTTON:
      ecs.RegisterComponent<GE::Component::GE_Button>();
      break;
    case COMPONENT_TYPES::ANCHOR:
      ecs.RegisterComponent<GE::Component::Anchor>();
      break;
    case COMPONENT_TYPES::CARD:
      ecs.RegisterComponent<GE::Component::Card>();
      break;
    case COMPONENT_TYPES::CARD_HOLDER:
      ecs.RegisterComponent<GE::Component::CardHolder>();
      break;
    case COMPONENT_TYPES::CARD_HOLDER_ELEM:
      ecs.RegisterComponent<GE::Component::CardHolderElem>();
      break;
    default:
      std::ostringstream oss{};
      oss << "Trying to register unknown component type, " << " update function: ObjectFactory::RegisterComponentsAndSystems()";
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      break;
    }
  }

  // Register systems
  std::string const systemsFile{ Assets::AssetManager::GetInstance().GetConfigData<std::string>("Systems") };
  std::vector<std::pair<std::string, ECS::ComponentSignature>> const systems{ Serialization::Deserializer::DeserializeSystems(systemsFile) };

  for (std::pair<std::string, ECS::ComponentSignature> const& elem : systems)
  {
    std::unordered_map<std::string, ECS::SYSTEM_TYPES>::const_iterator iter{ ECS::stringToSystems.find(elem.first) };
    if (iter == stringToSystems.cend())
    {
      throw Debug::Exception<std::ifstream>(Debug::LEVEL_ERROR, ErrMsg("Unable to register system " + elem.first));
    }
    RegisterSystemWithEnum(iter->second, elem.second);
  }
}

void ObjectFactory::RegisterSystemWithEnum(ECS::SYSTEM_TYPES name, ECS::ComponentSignature sig) const
{
  switch (name)
  {
  case SYSTEM_TYPES::COLLISION:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::CollisionSystem>();
    RegisterComponentsToSystem<Systems::CollisionSystem>(sig);
    break;
  case SYSTEM_TYPES::DRAGGABLE_OBJECT:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::DraggableObjectSystem>();
    RegisterComponentsToSystem<Systems::DraggableObjectSystem>(sig);
    break;
  case SYSTEM_TYPES::PHYSICS:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::PhysicsSystem>();
    RegisterComponentsToSystem<Systems::PhysicsSystem>(sig);
    break;
  case SYSTEM_TYPES::PLAYER_CONTROLLER:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::ScriptSystem>();
    RegisterComponentsToSystem<Systems::ScriptSystem>(sig);
    break;
  case SYSTEM_TYPES::RENDERING:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::RenderSystem>();
    RegisterComponentsToSystem<Systems::RenderSystem>(sig);
    break;
  case SYSTEM_TYPES::SPRITE_ANIM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::SpriteAnimSystem>();
    RegisterComponentsToSystem<Systems::SpriteAnimSystem>(sig);
    break;  
  case SYSTEM_TYPES::POST_ROOT_TRANSFORM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::PostRootTransformSystem>();
    RegisterComponentsToSystem<Systems::PostRootTransformSystem>(sig);
    break;
  case SYSTEM_TYPES::PRE_ROOT_TRANSFORM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::PreRootTransformSystem>();
    RegisterComponentsToSystem<Systems::PreRootTransformSystem>(sig);
  case SYSTEM_TYPES::ENEMY_SYSTEM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::EnemySystem>();
    RegisterComponentsToSystem<Systems::EnemySystem>(sig);
    break;
  case SYSTEM_TYPES::TEXT_RENDER:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::TextRenderSystem>();
    RegisterComponentsToSystem<Systems::TextRenderSystem>(sig);
    break;
  case SYSTEM_TYPES::TWEEN_SYSTEM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::TweenSystem>();
    RegisterComponentsToSystem<Systems::TweenSystem>(sig);
    break;
  case SYSTEM_TYPES::GAME_SYSTEMS:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::GameSystem>();
    RegisterComponentsToSystem<Systems::GameSystem>(sig);
    break;
  case SYSTEM_TYPES::AUDIO_SYSTEM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::AudioSystem>();
    RegisterComponentsToSystem<Systems::AudioSystem>(sig);
    break;
  case SYSTEM_TYPES::OBJECT_ANCHOR_SYSTEM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::ObjectAnchorSystem>();
    RegisterComponentsToSystem<Systems::ObjectAnchorSystem>(sig);
    break;
  case SYSTEM_TYPES::BUTTON_SYSTEM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::ButtonSystem>();
    RegisterComponentsToSystem<Systems::ButtonSystem>(sig);
    break;
  case SYSTEM_TYPES::BUTTON_SCRIPT_SYSTEM:
    EntityComponentSystem::GetInstance().RegisterSystem<Systems::ButtonScriptSystem>();
    RegisterComponentsToSystem<Systems::ButtonScriptSystem>(sig);
    break;
  default:
    std::ostringstream oss{};
    oss << "Trying to register unknown system type, " << " update function: ObjectFactory::RegisterSystemWithEnum()";
    throw Debug::Exception<ObjectFactory>(Debug::LEVEL_WARN, ErrMsg(oss.str()));
    break;
  }
}
