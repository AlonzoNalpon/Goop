#pragma once
#include "../ECS/SystemTypes.h"

namespace GE
{
  namespace ObjectFactory
  {
    constexpr size_t TOTAL_SYSTEMS{ 4 };

    using ComponentMap = std::unordered_map<GE::ECS::COMPONENT_TYPES, std::string>;
    using SystemMap = std::unordered_map<GE::ECS::SYSTEM_TYPES, std::string>;

    using SystemSignature = std::bitset <TOTAL_SYSTEMS>;

    struct Prefab
    {
      SystemSignature m_systemSignature;
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