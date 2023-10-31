/*!*********************************************************************
\file   ObjectStructs.h
\author loh.j@digipen.edu
\date   28 September 2023
\brief
  Contains the structs required for ObjectFactory.cpp

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "../ECS/SystemTypes.h"
#include <any>

namespace GE
{
  namespace ObjectFactory
  {
    using ComponentMap = std::unordered_map<GE::ECS::COMPONENT_TYPES, std::string>;

    struct ObjectData
    {
      ObjectData() = default;
      ObjectData(ECS::EntityID parent) 
        : m_components{}, m_childEntities{}, m_parent { parent }, m_componentSignature{} {}

      ComponentMap m_components;
      std::vector<ECS::Entity> m_childEntities;
      ECS::EntityID m_parent;
      ECS::ComponentSignature m_componentSignature;
    };
  }
}