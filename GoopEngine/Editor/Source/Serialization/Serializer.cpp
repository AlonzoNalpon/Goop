#include <pch.h>
#include "Serializer.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <Component/Components.h>
#include <ObjectFactory/ObjectFactory.h>
#include <rttr/enumeration.h>

namespace GE
{
  namespace Serialization
  {
    const char Serializer::JsonNameKey[]          = "Name";
    const char Serializer::JsonParentKey[]        = "Parent";
    const char Serializer::JsonChildEntitiesKey[] = "Child Entities";
    const char Serializer::JsonComponentsKey[]    = "Components";

    void Serializer::SerializeAny(std::string const& filename, rttr::variant object)
    {
      std::ofstream ofs{ filename };
      if (!ofs)
      {
        GE::Debug::ErrorLogger::GetInstance().LogError("Unable to serialize scene into " + filename);
      }

      rapidjson::Document document{ rapidjson::kArrayType };
      rapidjson::Value data{ SerializeBasedOnType(object, document.GetAllocator()) };
      document.CopyFrom(data.Move(), document.GetAllocator());

      rapidjson::OStreamWrapper osw{ ofs };
      rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
      document.Accept(writer);

      // clean up
      ofs.close();
      m_oldToNewIDs.clear();
    }

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

      std::unordered_map<ECS::Entity, ECS::Entity> idToNewId{ ObjectFactory::ObjectFactory::GetInstance().GenerateNewIDs() };
      
      rapidjson::Document document{ rapidjson::kArrayType };
      m_oldToNewIDs = std::move(ObjectFactory::ObjectFactory::GetInstance().GenerateNewIDs());
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

      // clean up
      ofs.close();
      m_oldToNewIDs.clear();
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

      std::ostringstream oss{};
      std::string const enumString = rttr::type::get<ECS::COMPONENT_TYPES>().get_enumeration().value_to_name(type).to_string();
      oss << "Trying to get unsupported component type (" << enumString << ") from Entity " << id;
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
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
      GLuint const texHandle{ gE.textureManager.GetTexture(data.m_spriteData.texture).textureHandle };
      rapidjson::Value jsonVal{};
      jsonVal.SetString(gE.textureManager.GetTextureName(texHandle).c_str(), allocator);

      return jsonVal;
    }

    rapidjson::Value Serializer::SerializeBasicTypes(rttr::type const& valueType, rttr::variant const& value, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value jsonVal{ rapidjson::kNullType };
      if (valueType == rttr::type::get<int>())
        jsonVal.SetInt(value.to_int());
      else if (valueType == rttr::type::get<double>())
        jsonVal.SetDouble(value.to_double());
      else if (valueType == rttr::type::get<const char*>())
        jsonVal.SetString(value.to_string().c_str(), allocator);
      else if (valueType == rttr::type::get<bool>())
        jsonVal.SetBool(value.to_bool());
      else if (valueType == rttr::type::get<unsigned>())
        jsonVal.SetUint(value.to_uint32());
      else if (valueType == rttr::type::get<char>())
        jsonVal.SetBool(value.to_bool());
      else if (valueType == rttr::type::get<float>())
        jsonVal.SetFloat(value.to_float());
      else if (valueType == rttr::type::get<uint64_t>())
        jsonVal.SetUint64(value.to_uint64());
      else if (valueType == rttr::type::get<uint16_t>())
        jsonVal.SetInt(value.to_uint16());
      else if (valueType == rttr::type::get<uint8_t>())
        jsonVal.SetInt(value.to_uint8());
      else
      {
        std::ostringstream oss{};
        oss << "Trying to deserialize unsupported basic type: " << valueType.get_name() << " with value " << value.to_string();
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
        jsonVal.SetNull();
      }

      return jsonVal;
    }

    rapidjson::Value Serializer::SerializeClassTypes(rttr::type const& valueType, rttr::variant const& value, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value jsonVal{ rapidjson::kObjectType };
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
      else if (valueType == rttr::type::get<GE::Math::Vec2>())
      {
        jsonVal.SetString(valueType.get_method("ToString").invoke(value).to_string().c_str(),
          allocator);
      }
      else if (valueType == rttr::type::get<std::deque<Math::dVec3>>())
      {
        jsonVal = SerializeTweenQueue(value.get_value<std::deque<Math::dVec3>>(), allocator);
      }
      else if (valueType == rttr::type::get<Component::DragForce>())
      {
        for (auto const& prop : value.get_type().get_properties())
        {
          rapidjson::Value innerProp{ SerializeBasedOnType(prop.get_value(value), allocator) };
          rapidjson::Value innerKey{ prop.get_name().to_string().c_str(), allocator };
          jsonVal.AddMember(innerKey, innerProp, allocator);
        }

        return jsonVal;
      }
      else if (valueType == rttr::type::get<std::vector<Component::LinearForce>>())
      {
        jsonVal.SetArray();
        for (rttr::variant const& force : value.get_value<std::vector<Component::LinearForce>>())
        {
          //rttr::instance forceInstance{ force };
          rapidjson::Value forceJson{ rapidjson::kObjectType };
          for (auto const& prop : force.get_type().get_properties())
          {
            rapidjson::Value innerVal{ SerializeBasedOnType(prop.get_value(value), allocator) };
            rapidjson::Value jsonKey{ prop.get_name().to_string().c_str(), allocator };
            forceJson.AddMember(jsonKey, innerVal, allocator);
          }
          jsonVal.PushBack(forceJson, allocator);
        }

        return jsonVal;
      }
      else
      {
        jsonVal.SetObject();
        for (auto const& prop : valueType.get_properties())
        {
          rapidjson::Value propVal{ rapidjson::kNullType };
          rapidjson::Value propKey{ prop.get_name().to_string().c_str(), allocator };

          propVal = SerializeBasedOnType(prop.get_value(value), allocator);
          jsonVal.AddMember(propKey, propVal, allocator);
        }
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
        rapidjson::Value jsonVal{ rapidjson::kNullType };
        rapidjson::Value jsonKey{ prop.get_name().to_string().c_str(), allocator };

        rttr::variant value{ prop.get_value(instance) };

        if (instance.get_type() == rttr::type::get<Component::SpriteAnim>())
        {
          jsonVal.SetString(Graphics::GraphicsEngine::GetInstance().animManager.GetAnimName(value.to_uint32()).c_str(), allocator);
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
            jsonVal = SerializeBasedOnType(value, allocator).Move();
          }
        }
        else
        {
          jsonVal = SerializeBasedOnType(value, allocator).Move();
        }

        compInner.AddMember(jsonKey, jsonVal, allocator);
      }
      comp.AddMember(compName, compInner, allocator);

      return comp;
    }

    rapidjson::Value Serializer::SerializeEnumType(rttr::type const& valueType, rttr::variant const& object, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value strJson{ rapidjson::kNullType };
      strJson.SetString(valueType.get_enumeration().value_to_name(object).to_string().c_str(), allocator);

      return strJson;
    }

    rapidjson::Value Serializer::SerializeBasedOnType(rttr::variant const& object, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value value{ rapidjson::kNullType };
      // if object is of basic type
      rttr::type valueType{ object.get_type() };
      rttr::type wrappedType{ valueType.is_wrapper() ? valueType.get_wrapped_type() : valueType };

      if (wrappedType.is_arithmetic())
      {
        value.SetObject();
        value = SerializeBasicTypes(wrappedType, object, allocator).Move();
      }
      else if (wrappedType.is_enumeration()) // if enum
      {
        value.SetObject();
        value = SerializeEnumType(wrappedType, object, allocator).Move();
      }
      else if (wrappedType.is_sequential_container())
      {
        value.SetArray();
        SerializeSequentialContainer(object, value, allocator);
      }
      else  // if class type
      {
        value.SetObject();
        value = SerializeClassTypes(wrappedType, object, allocator).Move();
      }

      return value;
    }

    void Serializer::SerializeSequentialContainer(rttr::variant const& object,
      rapidjson::Value& jsonArray, rapidjson::Document::AllocatorType& allocator)
    {
      auto containerIter = object.create_sequential_view();
      for (const auto& elem : containerIter)
      {
        // if underlying element is still a container, recurse
        if (elem.is_sequential_container())
        {
          SerializeSequentialContainer(elem, jsonArray, allocator);
        }
        else
        {
          rttr::variant wrappedVal = elem.extract_wrapped_value();
          // else serialize element normally
          jsonArray.PushBack(SerializeBasedOnType(wrappedVal, allocator).Move(), allocator);
        }
      }
    }
  } // namespace Serialization
} // namespace GE