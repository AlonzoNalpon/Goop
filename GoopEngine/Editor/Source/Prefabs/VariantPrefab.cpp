/*!*********************************************************************
\file   VariantPrefab.cpp
\author chengen.lau\@digipen.edu
\date   12-December-2023
\brief
  Contains the definition of the struct encapsulating deserialized
  prefab data. It is used during creation of entities from prefabs and
  when editing prefabs in the prefab editor. The implementation makes
  use of RTTR library to store components as rttr::variant objects.
  Each prefab also allows for multiple layers of components.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "VariantPrefab.h"
#include <ObjectFactory/ObjectFactory.h>

using namespace GE;
using namespace Prefabs;

PrefabSubData::PrefabSubData() : m_parent{ BasePrefabId } {}

PrefabSubData::PrefabSubData(std::string name, ECS::Entity id, ECS::Entity parent) :
  m_name{ std::move(name) }, m_components{}, m_id{ id }, m_parent{ parent } {}

ECS::Entity PrefabSubData::Construct() const
{
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
  ECS::Entity const entity{ ecs.CreateEntity() };

  ObjectFactory::ObjectFactory::GetInstance().AddComponentsToEntity(entity, m_components);
  ecs.SetEntityName(entity, m_name);

  return entity;
}

#ifdef PREFAB_V2
VariantPrefab::VariantPrefab(std::string name, unsigned version) :
  m_name{ std::move(name) }, m_objects{}, m_components{}, m_version{ version } {}

void VariantPrefab::Clear() noexcept
{
  m_name.clear();
  m_components.clear();
  m_objects.clear();
  m_version = 0;
}

ECS::Entity VariantPrefab::Construct() const
{
  std::unordered_map<ECS::Entity, ECS::Entity> idsToEntities;
  idsToEntities.reserve(m_objects.size() + 1);
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };

  // first, create the base entity
  ECS::Entity const entity{ ecs.CreateEntity() };
  ObjectFactory::ObjectFactory::GetInstance().AddComponentsToEntity(entity, m_components);
  ecs.SetEntityName(entity, m_name);
  idsToEntities.emplace(PrefabSubData::BasePrefabId, entity);   // map base ID to this entity ID

  // then, create child entities and map IDs
  for (PrefabSubData const& obj : m_objects)
  {
    idsToEntities.emplace(obj.m_id, obj.Construct());
  }

  // establish the hierarchy
  for (PrefabSubData const& obj : m_objects)
  {
    ECS::Entity const& child{ idsToEntities[obj.m_id] }, parent{ idsToEntities[obj.m_parent] };
    ecs.SetParentEntity(child, parent);
    ecs.AddChildEntity(parent, child);
  }

  return entity;
}
#else
VariantPrefab::VariantPrefab() : m_components{}, m_name{ "Empty" } {}

VariantPrefab::VariantPrefab(std::string name, unsigned version) :
  m_name{ std::move(name) }, m_components{}, m_version{ version } {}

void VariantPrefab::Clear() noexcept
{
  m_name.clear();
  m_components.clear();
  m_version = 0;
}
#endif
