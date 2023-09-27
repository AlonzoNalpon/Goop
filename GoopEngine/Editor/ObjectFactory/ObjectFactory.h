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
      GE::ECS::Entity SpawnPrefab(const std::string& key) const;
      GE::ECS::Entity CreateObject(ObjectData data) const;

      void CloneObject(ECS::Entity obj, Math::dVec2&& newPos);

      void JoelTest();
      int LoadObject() const;
      void ObjectJsonLoader(const std::string& json_path);

      static void ObjectFactoryTest();

    private:
      void RegisterObject(GE::ECS::Entity object) const;
      void DeserializePrefab(const std::string& filepath);
      void RegisterObjectToSystems(GE::ECS::Entity object, ECS::SystemSignature signature) const;
      void CloneComponents(GE::ECS::Entity destObj, GE::ECS::Entity srcObj) const;
      GE::ECS::SystemSignature GetObjectSystemSignature(GE::ECS::Entity obj) const;

      template <typename T, typename Signature>
      void SetBitIfFound(ECS::Entity entity, Signature& sig, ECS::SYSTEM_TYPES type) const
      {
        std::set<ECS::Entity>& entities{ ECS::EntityComponentSystem::GetInstance().GetSystem<T>()->GetEntities() };
        if (entities.find(entity) != entities.end()) { sig[static_cast<unsigned>(type)] = true; }
      }

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
