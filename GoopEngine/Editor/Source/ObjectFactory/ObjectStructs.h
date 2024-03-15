/*!*********************************************************************
\file       ObjectStructs.h
\author     loh.j\@digipen.edu
\co-author  chengen.lau\@digipen.edu
\date       28 September 2023
\brief
    Contains the structs used to store deserialized data for entities.
    Used for loading of scenes, creating and duplicating of entities,
    and in editing-related engine tools.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <rttr/type.h>

namespace GE
{
  namespace ObjectFactory
  {
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