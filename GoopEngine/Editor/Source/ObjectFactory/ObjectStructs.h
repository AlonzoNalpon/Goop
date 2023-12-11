/*!*********************************************************************
\file       ObjectStructs.h
\author     loh.j\@digipen.edu
\co-author  chengen.lau\@digipen.edu
\date       28 September 2023
\brief
    Contains the structs used to store deserialized data for entities
    and prefabs. Used for loading of scenes, creating and duplicating
    of entities, as well as editing tools in the engine.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <ECS/SystemTypes.h>
#include <rttr/type.h>

#define PREFAB_V2

namespace GE
{
  namespace ObjectFactory
  {
    // struct encapsulating deserialized prefab data
    // components are stored in an std::vector of rttr::variants
    struct VariantPrefab
    {
      VariantPrefab() : m_components{}, m_name{ "Empty" } {}
      VariantPrefab(std::string name, unsigned version = 0) :
        m_name{ std::move(name) }, m_components{}, m_version { version } {}

      std::string m_name;
      std::vector<rttr::variant> m_components;
      unsigned m_version;

      void Clear() noexcept
      {
        m_name.clear();
        m_components.clear();
        m_version = 0;
      }
    };

#ifdef PREFAB_V2
    struct PrefabSubData
    {
      PrefabSubData() : m_parent{ BasePrefabId } {}
      PrefabSubData(std::string name, ECS::Entity id, ECS::Entity parent = BasePrefabId) :
        m_name{ std::move(name) }, m_components{}, m_id{ id }, m_parent { parent } {}

      void AddComponent(rttr::variant const& comp) { m_components.emplace_back(comp); }
      void AddComponent(rttr::variant&& comp) { m_components.emplace_back(std::move(comp)); }

      std::string m_name;
      std::vector<rttr::variant> m_components;
      ECS::Entity m_id, m_parent;

      // id of the first layer of the prefab
      static ECS::Entity const BasePrefabId = 0;
    };

    struct VariantPrefab2
    {
      VariantPrefab2() = default;
      VariantPrefab2(std::string name, unsigned version = 0) :
        m_name{ std::move(name) }, m_objects{}, m_components {}, m_version{ version } {}

      inline bool HasChildComponents() const noexcept { return m_objects.empty(); }
      void Clear() noexcept { m_name.clear(); m_components.clear(); m_objects.clear(); m_version = 0; }

      std::string m_name;
      std::vector<PrefabSubData> m_objects;
      std::vector<rttr::variant> m_components;
      unsigned m_version;
    };
#endif

    // struct encapsulating deserialized data of an entity
    // this is pre-creation of the entity where components
    // are stored as rttr::variants in a vector
    struct VariantEntity
    {
      VariantEntity() = default;
      VariantEntity(std::string name, ECS::Entity parent = ECS::INVALID_ID, bool active = true)
        : m_name{ std::move(name) }, m_components{}, m_childEntities{}, m_parent{ parent }, m_isActive{ active } {}

      std::string m_name, m_prefab;
      std::vector<rttr::variant> m_components;
      std::vector<ECS::Entity> m_childEntities;
      ECS::Entity m_parent = ECS::INVALID_ID;
      bool m_isActive;
    };
  }
}