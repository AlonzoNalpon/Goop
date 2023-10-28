#include <pch.h>
#include "Serializer.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <Component/Components.h>
#include <ObjectFactory/ObjectFactory.h>

namespace GE
{
  namespace Serialization
  {
    const char Serializer::JsonNameKey[]          = "Name";
    const char Serializer::JsonParentKey[]        = "ParentID";
    const char Serializer::JsonChildEntitiesKey[] = "Child Entities";
    const char Serializer::JsonComponentsKey[]    = "Components";

    rapidjson::Value Serializer::SerializeEntity(ECS::Entity id, std::vector<ECS::Entity> const& childIDs, rapidjson::Document::AllocatorType & allocator)
    {
      rapidjson::Value entity{ rapidjson::kObjectType };
      rapidjson::Value entityName{};
      entityName.SetString(ECS::EntityComponentSystem::GetInstance().GetEntityName(id).c_str(), allocator);
      entity.AddMember(JsonNameKey, entityName, allocator);

      rapidjson::Value jsonParent{ rapidjson::kNullType };
      ECS::Entity const parentID{ ECS::EntityComponentSystem::GetInstance().GetParentEntity(id) };
      if (parentID != ECS::INVALID_ID)
      {
        jsonParent.SetUint(m_oldToNewIDs[parentID]);
      }
      entity.AddMember(JsonParentKey, jsonParent, allocator);

      rapidjson::Value childrenArr{ rapidjson::kArrayType };
      for (ECS::Entity const& child : childIDs)
      {
        rapidjson::Value childJson{};
        childJson.SetUint(child);
        childrenArr.PushBack(childJson, allocator);
      }
      entity.AddMember(JsonChildEntitiesKey, childrenArr, allocator);

      rapidjson::Value compArr{ rapidjson::kArrayType };
      for (ECS::COMPONENT_TYPES i{ static_cast<ECS::COMPONENT_TYPES>(0) }; i < ECS::COMPONENT_TYPES::COMPONENTS_TOTAL; ++i)
      {
        rttr::instance compInstance{ GetEntityComponent(id, i) };
        // skip if component wasn't found
        if (!compInstance.is_valid()) { continue; }

        rapidjson::Value comp{ SerializeComponent(compInstance, allocator) };
        compArr.PushBack(comp, allocator);
      }
      entity.AddMember(JsonComponentsKey, compArr, allocator);
      return entity;
    }

    void Serializer::SerializeScene(std::string const& filename)
    {
      std::ofstream ofs{ filename };
      if (!ofs)
      {
        GE::Debug::ErrorLogger::GetInstance().LogError("Unable to serialize scene into " + filename);
      }
      ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };

      std::unordered_map<ECS::Entity, ECS::Entity> idToNewId{ ObjectFactory::ObjectFactory::GetInstance().MapEntityIDs() };
      
      rapidjson::Document document{ rapidjson::kArrayType };
      m_oldToNewIDs = std::move(ObjectFactory::ObjectFactory::GetInstance().MapEntityIDs());
      for (ECS::Entity entity : ecs.GetEntities())
      {
        std::set<ECS::Entity> const& childIDs{ ecs.GetChildEntities(entity) };
        std::vector<ECS::Entity> newChildIDs{};
        newChildIDs.reserve(childIDs.size());
        for (ECS::Entity const& child : childIDs)
        {
          newChildIDs.emplace_back(m_oldToNewIDs[child]);
        }
        rapidjson::Value entityJson{ SerializeEntity(entity, newChildIDs, document.GetAllocator())};
        document.PushBack(entityJson, document.GetAllocator());
      }

      rapidjson::OStreamWrapper osw{ ofs };
      rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
      document.Accept(writer);

      ofs.close();

    }

    void Serializer::SerializeSystems(std::string const& json)
    {
      rapidjson::Document document{};
      std::ofstream ofs{ json };
      if (!ofs)
      {
        throw Debug::Exception<std::ifstream>(Debug::LEVEL_ERROR, ErrMsg("Unable to create output file " + json));
      }
      
      document.SetObject();
      std::map<int, const char*> const& systems{ ECS::EntityComponentSystem::GetInstance().GetSystemIndexes() };
      std::unordered_map<const char*, ECS::ComponentSignature> const& signatures{ ECS::EntityComponentSystem::GetInstance().GetSystemSignatures() };
      // loop through systems
      for (auto const& elem : systems)
      {
        rapidjson::Value compArr{ rapidjson::kArrayType };

        // get component names and append to rapidjson array
        auto const iter{ signatures.find(elem.second) };
        if (iter != signatures.cend())
        {
          std::vector<std::string> components{ ECS::ComponentSignatureToString(iter->second) };
          for (std::string const& str : components)
          {
            rapidjson::Value compName{ str.c_str(), document.GetAllocator() };
            compArr.PushBack(compName, document.GetAllocator());
          }
        }
        else
        {
          std::ostringstream oss{};
          oss << "Unable to find " << elem.second << " in system signatures";
          Debug::ErrorLogger::GetInstance().LogError(oss.str());
        }
        // truncate front part of system class type name to extract the system name 
        // (extract "PlayerControllerSystem" from "class GE::Systems::PlayerControllerSystem")
        std::string const sysName{ elem.second };
        rapidjson::Value key{ sysName.substr(sysName.find_last_of(':') + 1, std::string::npos).c_str(), document.GetAllocator()};
        document.AddMember(key, compArr, document.GetAllocator());
      }

      rapidjson::OStreamWrapper osw{ ofs };
      rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
      document.Accept(writer);
      ofs.close();
    }

    std::vector<std::pair<std::string, ECS::ComponentSignature>> Serializer::DeserializeSystems(std::string const& json)
    {
      std::ifstream ifs{ json };
      if (!ifs)
      {
        throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to read " + json));
      }

      rapidjson::Document document{};
      // parse into document object
      rapidjson::IStreamWrapper isw{ ifs };
      if (document.ParseStream(isw).HasParseError())
      {
        ifs.close();
        throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to parse " + json));
      }

      // check if root is object
      if (!document.IsObject())
      {
        ifs.close();
        throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg(json + ": root is not an object"));
      }

      std::vector<std::pair<std::string, ECS::ComponentSignature>> ret;
      // iterate through systems with key-array pairs
      for (auto const& system : document.GetObj())
      {
        ECS::ComponentSignature sig{};
        for (auto const& component : system.value.GetArray())
        {
          // set component's corresponding bit
          auto compType{ ECS::stringToComponents.find(component.GetString()) };
          if (compType == ECS::stringToComponents.cend())
          {
            ifs.close();
            std::string str{ "Invalid component in " };
            str += json + ": " + component.GetString();
            throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg(str));
          }

          sig[static_cast<unsigned>(compType->second)] = true;
        }
        ret.emplace_back(system.name.GetString(), sig); // add entry to container
      }

      ifs.close();
      return ret;
    }

    rttr::instance Serializer::GetEntityComponent(ECS::Entity id, ECS::COMPONENT_TYPES type)
    {
      switch (type)
      {
      case ECS::COMPONENT_TYPES::TRANSFORM:
        return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Transform>(id);
      case ECS::COMPONENT_TYPES::BOX_COLLIDER:
        return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::BoxCollider>(id);
      case ECS::COMPONENT_TYPES::MODEL:
        return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Model>(id);
      case ECS::COMPONENT_TYPES::SCRIPT_HANDLER:
        return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::ScriptHandler>(id);
      case ECS::COMPONENT_TYPES::SPRITE:
        return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Sprite>(id);
      case ECS::COMPONENT_TYPES::SPRITE_ANIM:
        return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::SpriteAnim>(id);
      case ECS::COMPONENT_TYPES::TWEEN:
        return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Tween>(id);
      case ECS::COMPONENT_TYPES::VELOCITY:
        return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Velocity>(id);
      case ECS::COMPONENT_TYPES::ENEMY_AI:
        return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::EnemyAI>(id);
      case ECS::COMPONENT_TYPES::DRAGGABLE:
        return *ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Draggable>(id);
      }

      return rttr::instance();
    }

    rapidjson::Value Serializer::SerializeScriptMap(std::map<std::string, MONO::Script> const& scripts, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value ret{ rapidjson::kArrayType };
      for (auto const& [s1, s2] : scripts)
      {
        rapidjson::Value jsonVal{};
        jsonVal.SetString(s1.c_str(), allocator);
        ret.PushBack(jsonVal, allocator);
      }
      return ret;
    }

    rapidjson::Value Serializer::SerializeTweenQueue(std::deque<Math::dVec3> tweens, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value ret{ rapidjson::kArrayType };
      while (!tweens.empty())
      {
        rapidjson::Value jsonVal{};
        jsonVal.SetString(tweens.front().ToString().c_str(), allocator);
        ret.PushBack(jsonVal, allocator);
        tweens.pop_front();
      }
      return ret;
    }

    rapidjson::Value Serializer::SerializeSpriteComponent(Component::Sprite const& data, rapidjson::Document::AllocatorType& allocator)
    {
      Graphics::GraphicsEngine const& gE{ Graphics::GraphicsEngine::GetInstance() };
      GLuint const texHandle{ gE.textureManager.GetTexture(data.spriteData.texture).textureHandle };
      rapidjson::Value jsonVal{};
      jsonVal.SetString(gE.textureManager.GetTextureName(texHandle).c_str(), allocator);

      return jsonVal;
    }

    rapidjson::Value Serializer::SerializeBasicTypes(rttr::type const& valueType, rttr::variant const& value, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value jsonVal{};
      if (valueType == rttr::type::get<int>())
      {
        jsonVal.SetInt(value.to_int());
      }
      else if (valueType == rttr::type::get<double>())
      {
        jsonVal.SetDouble(value.to_double());
      }
      else if (valueType == rttr::type::get<const char*>())
      {
        jsonVal.SetString(value.to_string().c_str(), allocator);
      }
      else if (valueType == rttr::type::get<bool>())
      {
        jsonVal.SetBool(value.to_bool());
      }
      else if (valueType == rttr::type::get<unsigned>())
      {
        jsonVal.SetUint(value.to_uint32());
      }
      else if (valueType == rttr::type::get<float>())
      {
        jsonVal.SetFloat(value.to_float());
      }
      else if (valueType == rttr::type::get<uint64_t>())
      {
        jsonVal.SetUint64(value.to_uint64());
      }
      else
      {
        std::ostringstream oss{};
        oss << "Trying to deserialize unknown basic type: " << valueType.get_name() << " with value " << value.to_string();
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
        jsonVal.SetNull();
      }

      return jsonVal;
    }

    rapidjson::Value Serializer::SerializeClassTypes(rttr::type const& valueType, rttr::variant const& value, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value jsonVal{};
      if (valueType == rttr::type::get<GE::Math::dVec3>())
      {
        jsonVal.SetString(valueType.get_method("ToString").invoke(value).to_string().c_str(),
          allocator);
      }
      else if (valueType == rttr::type::get<GE::Math::dVec2>())
      {
        jsonVal.SetString(valueType.get_method("ToString").invoke(value).to_string().c_str(),
          allocator);
      }
      else if (valueType == rttr::type::get<std::string>())
      {
        jsonVal.SetString(value.to_string().c_str(), allocator);
      }
      else if (valueType == rttr::type::get<std::deque<Math::dVec3>>())
      {
        jsonVal = SerializeTweenQueue(value.get_value<std::deque<Math::dVec3>>(), allocator);
      }
      else if (valueType == rttr::type::get<Component::DragForce>())
      {
        jsonVal.SetObject();
        for (auto const& prop : value.get_type().get_properties())
        {
          rapidjson::Value innerProp{};
          if (prop.get_type().is_arithmetic())  // if C basic types
          {
            innerProp = SerializeBasicTypes(prop.get_type(), value, allocator).Move();
          }
          else
          {
            innerProp = SerializeClassTypes(prop.get_type(), value, allocator).Move();
          }
          rapidjson::Value innerKey{ prop.get_name().to_string().c_str(), allocator };
          jsonVal.AddMember(innerKey, innerProp, allocator);
        }

        return jsonVal;
      }
      else if (valueType == rttr::type::get<std::vector<Component::LinearForce>>())
      {
        jsonVal.SetArray();
        for (Component::LinearForce const& force : value.get_value<std::vector<Component::LinearForce>>())
        {
          rttr::instance forceInstance{ force };
          rapidjson::Value forceJson{ rapidjson::kObjectType };
          for (auto const& prop : forceInstance.get_type().get_properties())
          {
            rapidjson::Value innerVal{};
            if (prop.get_type().is_arithmetic())  // if C basic types
            {
              innerVal = SerializeBasicTypes(prop.get_type(), value, allocator).Move();
            }
            else
            {
              innerVal = SerializeClassTypes(prop.get_type(), value, allocator).Move();
            }
            rapidjson::Value jsonKey{ prop.get_name().to_string().c_str(), allocator };
            forceJson.AddMember(jsonKey, innerVal, allocator);
          }
          jsonVal.PushBack(forceJson, allocator);
        }

        return jsonVal;
      }
      else
      {
        std::ostringstream oss{};
        oss << "Trying to deserialize unknown type: " << valueType.get_name() << " with value: " << value.to_string();
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
        jsonVal.SetNull();
      }

      return jsonVal;
    }

    rapidjson::Value Serializer::SerializeComponent(rttr::instance instance, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value comp{ rapidjson::kObjectType };
      rapidjson::Value compInner{ rapidjson::kObjectType };
      rapidjson::Value compName{};
      compName.SetString(instance.get_type().get_name().to_string().c_str(), allocator);
      for (auto const& prop : instance.get_type().get_properties())
      {
        rapidjson::Value jsonVal{};
        rapidjson::Value jsonKey{ prop.get_name().to_string().c_str(), allocator };

        rttr::variant value{ prop.get_value(instance) };

        if (instance.get_type() == rttr::type::get<Component::SpriteAnim>())
        {
          jsonVal.SetString(Graphics::GraphicsEngine::GetInstance().animManager.GetAnimName(value.to_uint32()).c_str(), allocator);
        }
        else if (instance.get_type() == rttr::type::get<Component::EnemyAI>())
        {
          jsonVal.SetUint(m_oldToNewIDs[value.get_value<Component::EnemyAI>().m_entityID]);
        }
        else if (prop.get_type().is_arithmetic())  // if C basic types
        {
          jsonVal = SerializeBasicTypes(prop.get_type(), value, allocator).Move();
        }
        else if (prop.get_type().is_class())  // else if custom types
        {
          // Handling special cases here (e.g. ScriptHandler's script map)
          if (instance.get_type() == rttr::type::get<Component::ScriptHandler>())
          {
            jsonVal = SerializeScriptMap(value.get_value<std::map<std::string, GE::MONO::Script> const&>(), allocator);
          }
          else if (instance.get_type() == rttr::type::get<Component::Sprite>())
          {
            jsonVal = SerializeSpriteComponent(value.get_value<Component::Sprite>(), allocator);
          }
          else
          {
            jsonVal = SerializeClassTypes(prop.get_type(), value, allocator).Move();
          }
        }

        compInner.AddMember(jsonKey, jsonVal, allocator);
      }
      comp.AddMember(compName, compInner, allocator);

      return comp;
    }

  } // namespace Serialization
} // namespace GE