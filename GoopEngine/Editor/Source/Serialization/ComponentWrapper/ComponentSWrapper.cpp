/*!*********************************************************************
\file   ComponentSWrapper.cpp
\author chengen.lau\@digipen.edu
\date   25-October-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ComponentWrapper.h"

using namespace GE;
using namespace Serialization;

ComponentSWrapper::ComponentSWrapper(const char* componentName)
{
  // set up the outer (base) layer of json
  m_data.SetObject();
  rapidjson::Value base{ rapidjson::kObjectType };
  rapidjson::Value key{ componentName, m_data.GetAllocator() };
  m_data.AddMember(key, base, m_data.GetAllocator());
}

void ComponentSWrapper::InsertBasicType(const char* key, rttr::variant value)
{
  rapidjson::Value jsonKey{ key, m_data.GetAllocator() };
  rapidjson::Value jsonVal{};
  rttr::type valueType{ value.get_type() };

  if (valueType == rttr::type::get<int>())
  {
    jsonVal.SetInt(value.to_int());
  }
  else if (valueType == rttr::type::get<double>())
  {
    jsonVal.SetDouble(value.to_int());
  }
  else if (valueType == rttr::type::get<GE::Math::dVec3>())
  {
    jsonVal.SetString(valueType.get_method("ToString").invoke(value).to_string().c_str(),
      m_data.GetAllocator());
  }
  else if (valueType == rttr::type::get<std::string>() || valueType == rttr::type::get<const char*>())
  {
    jsonVal.SetString(value.to_string().c_str(), m_data.GetAllocator());
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
    oss << "Trying to deserialize unknown basic type: " << valueType.get_name() << " with key: " << key;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    jsonVal.SetNull();
  }

  m_data.AddMember(jsonKey, jsonVal, m_data.GetAllocator());
}
