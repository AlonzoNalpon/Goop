/*!*********************************************************************
\file   PrefabGooStream.cpp
\author chengen.lau\@digipen.edu
\date   25-September-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include "PrefabGooStream.h"
#include <fstream>
#include <rapidjson/istreamwrapper.h>

using namespace GE::Serialization;

// MUST CORRESPOND TO KEYS IN .json
const char* const PrefabGooStream::JsonNameKey        = "Name";
const char* const PrefabGooStream::JsonSystemsKey     = "Systems";
const char* const PrefabGooStream::JsonComponentsKey  = "Components";

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
    std::ostringstream oss{};
    oss << json << " does not have a valid \"" << JsonNameKey << "\" field";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return m_status = false;
  }

  if (!data.HasMember(JsonComponentsKey) || !data[JsonSystemsKey].IsArray())
  {
    std::ostringstream oss{};
    oss << json << " does not have a valid \"" << JsonSystemsKey << "\" field";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return m_status = false;
  }

  if (!data.HasMember(JsonComponentsKey) || !data[JsonComponentsKey].IsArray())
  {
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
    if (!component.IsObject()) {
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
  std::cout << json << " successfully read" << "\n";
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

  for (auto const& i : data["Components"])
  {

  }

  return m_status = true;
}