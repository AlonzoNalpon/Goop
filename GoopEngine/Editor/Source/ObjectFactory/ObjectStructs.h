/*!*********************************************************************
\file   ObjectStructs.h
\author loh.j@digipen.edu
\date   28 September 2023
\brief
  Contains the structs required for ObjectFactory.cpp

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <ECS/SystemTypes.h>
#include <rttr/type.h>

namespace GE
{
  namespace ObjectFactory
  {
    struct VariantPrefab
    {
      VariantPrefab() : m_components{}, m_name{ "Empty" } {}
      VariantPrefab(std::string name) : m_components{}, m_name{ std::move(name) } {}

      std::vector<rttr::variant> m_components;
      std::string m_name;

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