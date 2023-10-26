#include <pch.h>
#include "GivingMyself90MinToFigureThisOut.h"
#include <ObjectFactory/ObjectFactory.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <Graphics/SpriteData.h>

namespace GE
{
  namespace ObjectFactory
  {
    rttr::instance GetEntityComponent(ECS::Entity id, ECS::COMPONENT_TYPES type)
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
      }

      return rttr::instance();
    }

  } // namespace ObjectFactory


  namespace Serialization
  {


    rapidjson::Value SerializeClassTypes(rttr::type const& valueType, rttr::variant const& value, rapidjson::Document::AllocatorType& allocator)
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
      else if (valueType == rttr::type::get<Graphics::SpriteData>())
      {
        unsigned const texID{ valueType.get_method("GetTextureHandle").invoke(value).to_uint32() };
        jsonVal.SetString(Graphics::GraphicsEngine::GetInstance().textureManager.GetTextureName(texID).c_str(), allocator);
      }
      /*else if (valueType == rttr::type::get<std::queue<GE::Math::dVec3>>())
      {
        jsonVal.SetArray();
        while (valueType.)
      }*/
      else
      {
        std::ostringstream oss{};
        oss << "Trying to deserialize unknown basic type: " << valueType.get_name() << " with value: " << value.to_string();
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
        jsonVal.SetNull();
      }

      return jsonVal;
    }
    
    rapidjson::Value SerializeBasicTypes(rttr::type const& valueType, rttr::variant const& value, rapidjson::Document::AllocatorType& allocator)
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

    rapidjson::Value SerializeComponent(rttr::instance instance, rapidjson::Document::AllocatorType& allocator)
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

        if (prop.get_type().is_arithmetic())  // if C basic types
        {
          jsonVal = SerializeBasicTypes(prop.get_type(), value, allocator).Move();
        }
        else if (prop.get_type().is_class())  // else if custom types
        {
          // Handling special cases here (e.g. ScriptHandler's script map)
          if (instance.get_type() == rttr::type::get<Component::ScriptHandler>())
          {
            jsonVal = Serialization::SerializeScriptMap(value.get_value<std::map<std::string, GE::MONO::Script> const&>(), allocator);
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

    rapidjson::Value SerializeEntity(ECS::Entity id, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value entity{ rapidjson::kObjectType };
      rapidjson::Value entityName{};
      entityName.SetString(ECS::EntityComponentSystem::GetInstance().GetEntityName(id).c_str(), allocator);
      entity.AddMember("Name", entityName, allocator);

      rapidjson::Value compArr{ rapidjson::kArrayType };
      for (ECS::COMPONENT_TYPES i{ static_cast<ECS::COMPONENT_TYPES>(0) }; i < ECS::COMPONENT_TYPES::COMPONENTS_TOTAL; ++i)
      {
        rttr::instance compInstance{ ObjectFactory::GetEntityComponent(id, i) };
        // skip if component wasn't found
        if (!compInstance.is_valid()) { continue; }
        
        rapidjson::Value comp{ SerializeComponent(compInstance, allocator) };
        compArr.PushBack(comp, allocator);
      }
      entity.AddMember("Components", compArr, allocator);
      return entity;
    }

    void SerializeScene(std::string const& filename)
    {
      std::ofstream ofs{ filename };
      if (!ofs)
      {
        GE::Debug::ErrorLogger::GetInstance().LogError("Unable to serialize scene into " + filename);
      }


      rapidjson::Document document{ rapidjson::kArrayType };
      for (ECS::Entity const& entity : ECS::EntityComponentSystem::GetInstance().GetEntities())
      {
        rapidjson::Value entityJson{ SerializeEntity(entity, document.GetAllocator()) };
        document.PushBack(entityJson, document.GetAllocator());
      }

      rapidjson::OStreamWrapper osw{ ofs };
      rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
      document.Accept(writer);

      ofs.close();

    }
  } // namespace Serialization
} // namespace GE