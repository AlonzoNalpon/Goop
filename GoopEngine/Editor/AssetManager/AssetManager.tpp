/*!*********************************************************************
\file   AssetManager.tpp
\author loh.j@digipen.edu
\date   28 September 2023
\brief
  Contains the template functions of GetConfigData.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

/*!*********************************************************************
\brief
  Gets the config data of the specific key.
\param
  const std::string& key (Key of the config data in the Config.json)
\return
  Data of the specific key converted to type T
************************************************************************/
template <typename T>
std::optional<T> GetConfigData(const std::string& key) const
{
  std::ostringstream oss{};
  oss << "Unabled to convert config data of key \"" << key << " \" to " << typeid(T).name();
  GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
  return std::nullopt;
}

template <>
std::optional<int> GetConfigData<int>(const std::string& key) const
{
  if (m_configData.find(key) == m_configData.end())
    return std::nullopt;

  #ifdef ASSET_MANAGER_DEBUG
  std::cout << "CONFIG DATA RETRIEVED: " << m_configData[key] << "\n";
  #endif

  return std::stoi(m_configData.at(key));
}

template <>
std::optional<unsigned> GetConfigData<unsigned>(const std::string& key) const
{
  if (m_configData.find(key) == m_configData.end())
    return std::nullopt;

  return std::stoul(m_configData.at(key));
}

template <>
std::optional<double> GetConfigData<double>(const std::string& key) const
{
  if (m_configData.find(key) == m_configData.end())
    return std::nullopt;

  return std::stod(m_configData.at(key));
}

template <>
std::optional<const char*> GetConfigData<const char*>(const std::string& key) const
{
  if (m_configData.find(key) == m_configData.end())
    return std::nullopt;

  return m_configData.at(key).c_str();
}

template <>
std::optional<std::string> GetConfigData<std::string>(const std::string& key) const
{
  if (m_configData.find(key) == m_configData.end())
    return std::nullopt;

  return m_configData.at(key);
}