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
      void LoadPrefabsFromFile();
      void RegisterComponentsAndSystems() const;
      GE::ECS::Entity SpawnPrefab(const std::string& key);
      GE::ECS::Entity CreateObject(ObjectData data);

      void JoelTest();
      int LoadObject();
      void ObjectJsonLoader(const std::string& json_path);

      static void ObjectFactoryTest();

    private:
      void RegisterObject(GE::ECS::Entity object);
      void DeserializePrefab(const std::string& filepath);
      void RegisterPrefab(GE::ECS::Entity object, ECS::SystemSignature signature);

      inline bool IsBitSet(ECS::ComponentSignature lhs, ECS::COMPONENT_TYPES rhs) const noexcept
      {
        return lhs[static_cast<unsigned>(rhs)];
      }
      inline bool IsBitSet(GE::ECS::SystemSignature lhs, ECS::SYSTEM_TYPES rhs) const noexcept
      {
        return lhs[static_cast<unsigned>(rhs)];
      }

      std::map<std::string, ObjectData> m_objects;
      std::unordered_map<std::string, PrefabData> m_prefabs;
    };
  }
}
