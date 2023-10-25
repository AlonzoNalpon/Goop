/*!*********************************************************************
\file   ComponentDWrapper.cpp
\author chengen.lau\@digipen.edu
\date   15-October-2023
\brief  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ComponentWrapper.h"

using namespace GE;
using namespace Serialization;

ComponentDWrapper::ComponentDWrapper(std::string const& componentData) : m_data{}
{
  if (m_data.Parse(componentData.c_str()).HasParseError())
  {
    throw Debug::Exception<ComponentDWrapper>(Debug::LEVEL_ERROR, ErrMsg("Unable to parse component data"));
  }
  m_ptr = &m_data.MemberBegin()->value;
}

// base subscript operator definition to fallback to
template <typename T>
T ComponentDWrapper::Get(const char* key) const
{
  std::ostringstream oss{};
  oss << "Trying to deserialize " << key << " into unsupported type " << typeid(T).name();
  GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
  return T();
}

template <>
bool ComponentDWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetBool();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to bool";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return false;
  }
}

template <>
double ComponentDWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetDouble();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to double";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return 0.0;
  }
}

template <>
int ComponentDWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetInt();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to int";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return 0;
  }
}

template <>
unsigned ComponentDWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetUint();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to unsigned";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return 0;
  }
}

template <>
size_t ComponentDWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetUint64();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to size_t";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return 0;
  }
}

template <>
const char* ComponentDWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetString();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to const char*";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return nullptr;
  }
}

template <>
std::string ComponentDWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetString();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to std::string";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return std::string();
  }
}

template <>
Math::Vec2 ComponentDWrapper::Get(const char* key) const
{
  try
  {
    Math::Vec2 vec{};
    return vec << (*m_ptr)[key].GetString();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to Math::Vec2";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return Math::Vec2();
  }
}

template <>
Math::dVec2 ComponentDWrapper::Get(const char* key) const
{
  try
  {
    Math::dVec2 vec{};
    return vec << (*m_ptr)[key].GetString();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to Math::dVec2";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return Math::dVec2();
  }
}

template<> Math::dVec3 ComponentDWrapper::Get(const char* key) const
{
  try
  {
    Math::dVec3 vec{};
    return vec << (*m_ptr)[key].GetString();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to Math::dVec3";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return Math::dVec3();
  }
}

template <>
std::queue<Math::dVec2> ComponentDWrapper::Get(const char* key) const
{
  try
  {
    std::queue<Math::dVec2> ret{};
    rapidjson::Value const& list{ (*m_ptr)[key] };
    for (auto const& elem : list.GetArray())
    {
      Math::dVec2 vec{};
      vec << elem.GetString();
      ret.emplace(std::move(vec));
    }

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to std::queue<Math::dVec2>";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}

template <>
std::queue<Math::dVec3> ComponentDWrapper::Get(const char* key) const
{
  try
  {
    std::queue<Math::dVec3> ret{};
    rapidjson::Value const& list{ (*m_ptr)[key] };
    for (auto const& elem : list.GetArray())
    {
      Math::dVec3 vec{};
      vec << elem.GetString();
      ret.emplace(std::move(vec));
    }

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to std::queue<Math::dVec3>";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}

template<> 
std::vector<std::pair<std::string, std::string>>
  ComponentDWrapper::Get(const char* key) const
{
  try
  {
    std::vector<std::pair<std::string, std::string>> ret{};
    rapidjson::Value const& list{ (*m_ptr)[key] };
    for (auto const& elem : list.GetObj())
    {
      ret.emplace_back(elem.name.GetString(), elem.value.GetString());
    }

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to std::vector<std::pair<std::string, std::string>>";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}

template<> Component::DragForce ComponentDWrapper::Get(const char* key) const
{
  try
  {
    Component::DragForce ret{};
    rapidjson::Value const& list{ (*m_ptr)[key] };
    ret.m_magnitude << list["m_magnitude"].GetString();
    ret.m_isActive = list["m_isActive"].GetBool();

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to Component::DragForce";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}

template<> std::vector<Component::LinearForce> ComponentDWrapper::Get(const char* key) const
{
  try
  {
    std::vector<Component::LinearForce> ret{};
    rapidjson::Value const& list{ (*m_ptr)[key] };
    for (auto const& elem : list.GetArray())
    {
      Math::dVec2 vec;
      vec << elem["m_magnitude"].GetString();
      ret.emplace_back(
        vec, elem["m_lifetime"].GetDouble(), elem["m_isActive"].GetBool()
      );
    }

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentDWrapper: Unable to convert value of key " << key
      << " to std::vector<Component::LinearForce>";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}