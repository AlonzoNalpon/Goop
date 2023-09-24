#pragma once
#include <pch.h>
#include "../ECS/ComponentTypes.h"
#include "../Serialization/Serialization.h"

namespace GE
{
  namespace ObjectFactory
  {
    //using MemberMap = std::map<std::string, std::string>;
    
    // Map holding serialized json data for each component
    using ComponentMap = std::map<GE::ECS::COMPONENT_TYPES, std::string>;

    struct ObjectData
    {
      GE::ECS::ComponentSignature m_signature;
      ComponentMap m_components;
    };

    class ObjectFactory : public Singleton<ObjectFactory>
    {
    public:
      void RegisterObject(GE::ECS::Entity object);


      GE::ECS::Entity CreateObject(ObjectData data);


      void JoelTest();


      int LoadObject();

      void ObjectJsonLoader(std::string json_path);

      static void ObjectFactoryTest();
    private:
      inline bool IsBitSet(ECS::ComponentSignature lhs, ECS::COMPONENT_TYPES rhs) const noexcept
      {
        return lhs[static_cast<unsigned>(rhs)];
      }

      std::map<std::string, ObjectData> m_objects;
    };
  }
}
