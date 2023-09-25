#pragma once
#include <pch.h>
#include "../ECS/ComponentTypes.h"
#include <rapidjson/document.h>
#include "ObjectStructs.h"
#include "../Serialization/Serialization.h"

namespace GE
{
  namespace ObjectFactory
  {
    class ObjectFactory : public Singleton<ObjectFactory>
    {
    public:
      void RegisterObject(GE::ECS::Entity object);

      void RegisterPrefab(GE::ECS::Entity object, SystemSignature signature);

      GE::ECS::Entity CreateObject(ObjectData data);

      GE::ECS::Entity SpawnPrefab(std::string);


      void JoelTest();

      int LoadObject();

      void ObjectJsonLoader(std::string json_path);

      static void ObjectFactoryTest();

    private:
      inline bool IsBitSet(ECS::ComponentSignature lhs, ECS::COMPONENT_TYPES rhs) const noexcept
      {
        return lhs[static_cast<unsigned>(rhs)];
      }
      inline bool IsBitSet(GE::ObjectFactory::SystemSignature lhs, ECS::SYSTEM_TYPES rhs) const noexcept
      {
        return lhs[static_cast<unsigned>(rhs)];
      }

      std::map<std::string, ObjectData> m_objects;
      std::map<std::string, Prefab> m_prefabs;

    };
  }
}
