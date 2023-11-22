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

#define RTTR_DESERIALIZE

namespace GE
{
  namespace ObjectFactory
  {
    using ComponentMap = std::unordered_map<GE::ECS::COMPONENT_TYPES, std::string>;

    struct ObjectData
    {
      ObjectData() = default;
      ObjectData(ECS::Entity parent, std::string name) 
        : m_components{}, m_name{ std::move(name) }, m_childEntities{}, m_parent{parent}, m_componentSignature{} {}

      ComponentMap m_components;
      std::string m_name;
      std::vector<ECS::Entity> m_childEntities;
      ECS::Entity m_parent = ECS::INVALID_ID;
      ECS::ComponentSignature m_componentSignature;
    };

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
      VariantEntity(std::string name, ECS::Entity parent)
        : m_name{std::move(name)}, m_components{}, m_childEntities{}, m_parent{parent} {}

      std::string m_name;
      std::vector<rttr::variant> m_components;
      std::vector<ECS::Entity> m_childEntities;
      ECS::Entity m_parent = ECS::INVALID_ID;
      //ECS::ComponentSignature m_componentSignature;
    };
  }
}