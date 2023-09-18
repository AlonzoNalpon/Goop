/*!*********************************************************************
\file   ConfigGooStream.cpp
\author chengen.lau\@digipen.edu
\date   18-September-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include "ConfigGooStream.h"
#include <fstream>
#include <rapidjson/istreamwrapper.h>
#ifdef _DEBUG
#include <iostream>
#endif

using namespace GE::Serialization;

// static variables
std::string const ConfigGooStream::ASSETS_DIR{ "../Assets/AssetsToLoadTest/" };

AssetGooStream::AssetGooStream(std::string const& json) : GooStream()
{
  Read(json);
}

bool AssetGooStream::Read(std::string const& json)
{
  std::ifstream ifs{ ASSETS_DIR + json };
  if (!ifs)
  {
    #ifdef _DEBUG
    std::cout << "Error: Unable to load " << ASSETS_DIR + json << "\n";
    #endif
    return m_status = false;
  }

  // parse into document object
  rapidjson::IStreamWrapper isw{ ifs };
  if (m_data.ParseStream(isw).HasParseError())
  {
    ifs.close();

    #ifdef _DEBUG
    std::cout << "JSON parse error: " << rapidjson::GetParseErrorFunc(m_data.GetParseError()) << "\n";
    #endif
    return m_status = false;
  }

  // if data is in the format of key-value pairs
  // i.e. "key": "value"
  if (!m_data.IsObject())
  {
    ifs.close();

    #ifdef _DEBUG
    std::cerr << "JSON parse error: " << ASSETS_DIR + json << " is not in the right format " << "\n";
    #endif
    return m_status = false;
  }

  m_elementCount = m_data.MemberCount();  // num. of pairs
  ifs.close();
  return m_status = true;

    #ifdef SERIALIZE_TEST
  std::cout << ASSETS_DIR + json << " successfully read" << "\n";
    #endif
}

bool AssetGooStream::Unload(container_type& container)
{
  if (!m_status) {
    #ifdef _DEBUG
    std::cout << "AssetGooStream corrupted before unload\n";
    #endif
    return false;
  }

  for (rapidjson::Value::ConstMemberIterator it{ m_data.MemberBegin() };
    it != m_data.MemberEnd(); ++it)
  {
    container[it->name.GetString()] = ASSETS_DIR + it->value.GetString();
  }

  return m_status = true;
}
