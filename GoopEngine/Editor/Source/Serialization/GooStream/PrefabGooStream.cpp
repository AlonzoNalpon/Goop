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
#include <Serialization/Deserializer.h>
#include <Serialization/Serializer.h>

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
  if (ifs.peek() == std::ifstream::traits_type::eof())
  {
    GE::Debug::ErrorLogger::GetInstance().LogMessage("Deserializing and read empty file. Ignoring checks");
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

  Deserializer::ScanJsonFileForMembers(data, 2, Serialization::Serializer::JsonNameKey, rapidjson::kStringType
                                              , Serialization::Serializer::JsonComponentsKey, rapidjson::kArrayType);

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
      std::ostringstream oss{};
      oss << "Unable to find component " << component.MemberBegin()->name.GetString();
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
      std::cout << oss.str() << "\n";
#endif
      continue;
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