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
    using SystemMap = std::unordered_map<GE::ECS::SYSTEM_TYPES, std::string>;

    struct PrefabData
    {
      ECS::SystemSignature m_systemSignature;
      GE::ECS::ComponentSignature m_componentSignature;
      ComponentMap m_components;
    };

    struct ObjectData
    {
      GE::ECS::ComponentSignature m_componentSignature;
      ComponentMap m_components;
    };
  }
}