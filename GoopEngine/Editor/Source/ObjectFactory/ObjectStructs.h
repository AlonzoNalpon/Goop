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

namespace GE
{
  namespace ObjectFactory
  {
    // struct encapsulating deserialized prefab data
    // components are stored in an std::vector of rttr::variants
    struct VariantPrefab
    {
      VariantPrefab() : m_components{}, m_name{ "Empty" } {}
      VariantPrefab(std::string name, unsigned version = 0) : m_components{}, m_name{ std::move(name) }, m_version{ version } {}

      std::vector<rttr::variant> m_components;
      std::string m_name;
      unsigned m_version;

      void Clear() noexcept
      {
        m_name.clear();
        m_components.clear();
      }
    };

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