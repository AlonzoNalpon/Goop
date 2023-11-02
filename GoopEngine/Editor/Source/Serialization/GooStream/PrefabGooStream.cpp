/*!*********************************************************************
\file   PrefabGooStream.cpp
\author chengen.lau\@digipen.edu
\date   25-September-2023
\brief  GooStream class used for serializing / deserializing of a prefab
        json file, together with the systems it is under, it's individual
        components, and their values. The class overrides only input
        functions.

        For the format of the json file, you can refer to
        Assets/Data/Prefabs/MineWorm.json
        For more details on usage, refer to GooStream.h
  
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "PrefabGooStream.h"
#include <rapidjson/istreamwrapper.h>
#include <ECS/SystemTypes.h>

using namespace GE::Serialization;

// MUST CORRESPOND TO KEYS IN .json
const char PrefabGooStream::JsonNameKey[]       = "Name";
const char PrefabGooStream::JsonComponentsKey[] = "Components";

PrefabGooStream::PrefabGooStream(std::string const& json) : GooStream(true)
{
  m_elements = 0;
  Read(json);
}

bool PrefabGooStream::Read(std::string const& json)
{
  std::ifstream ifs{ json };
  if (!ifs)
  {
    std::ostringstream oss{ "Unable to load " };
    oss << json;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << "PrefabGooStream: " << oss.str() << std::endl;
    #endif
    return m_status = false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };
  
  rapidjson::IStreamWrapper isw{ ifs };
  if (data.ParseStream(isw).HasParseError())
  {
    ifs.close();
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to read " + json);

    #ifdef _DEBUG
    std::cout << "PrefabGooStream: Unable to read " << json << std::endl;
    #endif
    return m_status = false;
  }

  if (!data.IsObject())
  {
    ifs.close();
    std::ostringstream oss{};
    oss << "PrefabGooStream: in " << json << ", root is not an object";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return m_status = false;
  }

  if (!data.HasMember(JsonNameKey) || !data[JsonNameKey].IsString())
  {
    ifs.close();
    std::ostringstream oss{};
    oss << json << " does not have a valid \"" << JsonNameKey << "\" field";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return m_status = false;
  }

  if (!data.HasMember(JsonComponentsKey) || !data[JsonComponentsKey].IsArray())
  {
    ifs.close();
    std::ostringstream oss{};
    oss << json << " does not have a valid \"" << JsonComponentsKey << "\" field";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return m_status = false;
  }

  rapidjson::Value const& componentArr{ data["Components"] };
  for (rapidjson::SizeType i{}; i < componentArr.Size(); ++i)
  {
    rapidjson::Value const& component{ componentArr[i] };
    if (!component.IsObject())
    {
      ifs.close();
      std::ostringstream oss{};
      oss << json << ": Component " << std::to_string(i) << " has invalid format";
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

      #ifdef _DEBUG
      std::cout << oss.str() << std::endl;
      #endif
      return m_status = false;
    }
  }

  #ifdef _DEBUG
  //std::cout << json << " successfully read" << "\n";
  #endif
  ifs.close();
  return m_status = true;
}

bool PrefabGooStream::Unload(container_type& object)
{
  if (!m_status) {
    std::ostringstream oss{ "PrefabGooStream corrupted before unloading into " };
    oss << typeid(object).name();
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  // set name of object
  object.first = data[JsonNameKey].GetString();

  // iterate through components array to
  // add json data to map and set object's component signature
  for (auto const& component : data[JsonComponentsKey].GetArray())
  {
    auto iter{ ECS::stringToComponents.find(component.MemberBegin()->name.GetString()) };
    if (iter == ECS::stringToComponents.cend())
    {
      std::string str{ "Unable to find component " };
      throw Debug::Exception<std::ifstream>(Debug::LEVEL_ERROR, ErrMsg(str + component.MemberBegin()->name.GetString()));
    }
    // set current component's bit in signature
    object.second.m_componentSignature[static_cast<unsigned>(iter->second)] = true;

    rapidjson::StringBuffer buffer{};
    writer_type<rapidjson::StringBuffer> writer{ buffer };
    component.Accept(writer);
    object.second.m_components[iter->second] = buffer.GetString();
  }

  return m_status = true;
}