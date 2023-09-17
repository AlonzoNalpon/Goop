#include "GooStream.h"
#include <fstream>
#include <rapidjson/istreamwrapper.h>
#ifdef _DEBUG
#include <iostream>
#endif

using namespace GE::Serialization;

// static variables
std::string const AssetGooStream::ASSETS_DIR{ "../Assets/AssetsToLoadTest/" };

void GooStream::Reset() noexcept
{
  m_data.Clear();
  m_elementCount = 0;
  m_status = true;
}

AssetGooStream::AssetGooStream(std::string const& json) : GooStream()
{
  std::ifstream ifs{ ASSETS_DIR + json };
  if (!ifs)
  {
    m_status = false;

    #ifdef _DEBUG
    std::cout << "Error: Unable to load " << ASSETS_DIR + json << "\n";
    #endif
    return;
  }

  // parse into document object
  rapidjson::IStreamWrapper isw{ ifs };
  if (m_data.ParseStream(isw).HasParseError())
  {
    m_status = false;
    ifs.close();

    #ifdef _DEBUG
    std::cout << "JSON parse error: " << rapidjson::GetParseErrorFunc(m_data.GetParseError()) << "\n";
    #endif
    return;
  }

  // if data is in the format of key-value pairs
  // i.e. "key": "value"
  if (!m_data.IsObject())
  {
    m_status = false;

    #ifdef _DEBUG
    std::cerr << "JSON parse error: " << ASSETS_DIR + json << " is not in the right format " << "\n";
    #endif
    return;
  }

  m_elementCount = m_data.MemberCount();  // num. of pairs
  m_status = true;
  ifs.close();

  #ifdef SERIALIZE_TEST
  std::cout << ASSETS_DIR + json << " successfully read" << "\n";
  #endif
}

bool AssetGooStream::Read(std::string const& json)
{ 
  Reset();
  AssetGooStream::AssetGooStream(json);

  return m_status;
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
