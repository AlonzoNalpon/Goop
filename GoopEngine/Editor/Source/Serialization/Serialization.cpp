#include <pch.h>
#include "Serialization.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

namespace GE
{
  namespace Serialization
  {
    void SerializeSystems(std::string const& json)
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

    std::vector<std::pair<std::string, ECS::ComponentSignature>> DeserializeSystems(std::string const& json)
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
          std::unordered_map<std::string, ECS::COMPONENT_TYPES>::const_iterator compType{ ECS::stringToComponents.find(component.GetString()) };
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

  } // namespace Serialization
} // namespace GE