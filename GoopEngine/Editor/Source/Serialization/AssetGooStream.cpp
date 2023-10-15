/*!*********************************************************************
\file   AssetGooStream.h
\author chengen.lau\@digipen.edu
\date   18-September-2023
\brief  GooStream class used for serializing / deserializing of an
        asset file (config, images etc.). The json format read by
        this stream must be in key-value pairs ("key": "value").
        It overrides both input and output functions.

        For the format of the json file, you can refer to
        Assets/Data/Config.json
        For more details on usage, refer to GooStream.h

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "AssetGooStream.h"
#include <fstream>
#include <rapidjson/istreamwrapper.h>

using namespace GE::Serialization;

AssetGooStream::AssetGooStream(std::string const& json) : GooStream(true)
{
  m_elements = 0;
  Read(json);
}

bool AssetGooStream::Read(std::string const& json)
{
  std::ifstream ifs{ json };
  if (!ifs)
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to read " + json);
    #ifdef _DEBUG
    std::cout << "AssetGooStream: Unable to read " + json << std::endl;
    #endif
    return m_status = false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  // parse into document object
  rapidjson::IStreamWrapper isw{ ifs };
  if (data.ParseStream(isw).HasParseError())
  {
    ifs.close();
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to read " + json);

    #ifdef _DEBUG
    std::cout << "AssetGooStream: Unable to read " << json << std::endl;
    #endif
    return m_status = false;
  }

  // if data is not in the format of key-value pairs
  // i.e. "key": "value"
  if (!data.IsObject())
  {
    ifs.close();
    std::ostringstream oss{};
    oss << "AssetGooStream: " << json << " is not in the right format ";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return m_status = false;
  }

  #ifdef SERIALIZE_TEST
  std::cout << json << " successfully read" << "\n";
  #endif
  ifs.close();
  m_elements = data.MemberCount();
  return m_status = true;
}

bool AssetGooStream::Read(container_type const& container)
{
  if (!m_status) {
    std::ostringstream oss{ "AssetGooStream corrupted before reading from " };
    oss << typeid(container).name();
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  for (container_type::value_type const& entry : container)
  {
    const char* key{ entry.first.c_str() };
    rapidjson::Value jsonKey{ key, data.GetAllocator() };
    rapidjson::Value value{ entry.second.c_str(), data.GetAllocator() };

    // Append to m_data or overwrite if key already exists
    if (!data.HasMember(key))
    {
      data.AddMember(jsonKey, value, data.GetAllocator());
    }
    else
    {
      data[key] = value;
    }
  }

  m_elements = data.Size();
  return m_status = true;
}

bool AssetGooStream::Unload(container_type& container)
{
  if (!m_status) {
    std::ostringstream oss{ "AssetGooStream corrupted before unloading into " };
    oss << typeid(container).name();
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  for (rapidjson::Value::ConstMemberIterator it{ data.MemberBegin() };
    it != data.MemberEnd(); ++it)
  {
    container[it->name.GetString()] = it->value.GetString();
  }

  return m_status = true;
}

bool AssetGooStream::Unload(std::string const& json, bool overwrite)
{
  if (!m_status) {
    std::ostringstream oss{ "AssetGooStream corrupted before unloading into " };
    oss << json;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  std::ofstream ofs{ json, ((overwrite) ? std::ios::out : std::ios::app) };
  if (!ofs)
  {
    std::ostringstream oss{ "Unable to create output file " };
    oss << json;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return m_status = false;
  }

  rapidjson::OStreamWrapper osw{ ofs };
  writer_type<rapidjson::OStreamWrapper> writer(osw);
  data.Accept(writer);

  ofs.close();
  return m_status = true;
}