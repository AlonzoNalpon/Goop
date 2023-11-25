#include <pch.h>
#include "ScriptInstance.h"
#include "ScriptManager.h"
/*!*********************************************************************
\file   Script.cpp
\author han.q\@digipen.edu
\date   28 September 2023
\brief
  Script Class to contain the data of a c# class.
  It will store the pointer to the MonoObject and its 4 main methods


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

using namespace GE;
using namespace MONO;




ScriptInstance::ScriptInstance(const std::string& scriptName, std::vector<void*>& arg) : m_scriptName{ scriptName }
{
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  m_scriptClass = sm->GetScriptClass(scriptName);
  m_classInst = sm->InstantiateClass(scriptName.c_str(), arg);
  m_onUpdateMethod = mono_class_get_method_from_name(m_scriptClass,"OnUpdate", 1);
  //m_onCreateMethod = mono_class_get_method_from_name(m_scriptClassInfo.m_scriptClass, "onCreate", 1);
}


ScriptInstance::ScriptInstance(const std::string& scriptName) : m_scriptName{ scriptName }
{
  
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  m_scriptClass = sm->GetScriptClass(scriptName);
  m_classInst = sm->InstantiateClass(scriptName.c_str());
  m_onUpdateMethod = mono_class_get_method_from_name(m_scriptClass, "OnUpdate", 1);
  GetFields();
  //m_onCreateMethod = mono_class_get_method_from_name(m_scriptClassInfo.m_scriptClass, "onCreate", 1);
}

template<typename T>
void ScriptFieldInstance<T>::SetMyScriptField(const std::string& className, const std::string& fieldName)
{
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  m_scriptField = sm->GetScriptField(className, fieldName);
}

void ScriptInstance::Clear()
{
  //mono_free(m_classInst);
}

void ScriptInstance::InvokeOnUpdate(double dt)
{
  if (m_onUpdateMethod)
  {
    void* param = &dt;
    mono_runtime_invoke(m_onUpdateMethod, m_classInst, &param, nullptr);
  }
}


void ScriptInstance::InvokeOnCreate()
{
  if (m_onCreateMethod)
  {
    mono_runtime_invoke(m_onCreateMethod, m_classInst,nullptr, nullptr);
  }
}

void ScriptInstance::GetFields()
{
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  ScriptClassInfo sci = sm->GetScriptClassInfo(m_scriptName);
	const auto& fields = sci.m_ScriptFieldMap;
	for (const auto& [fieldName, field] : fields)
	{

		if (field.m_fieldType == ScriptFieldType::Float)
		{
      
			float value = GetFieldValue<float>(field.m_classField);
      ScriptFieldInstance<float> test{ field,value };
      
      m_scriptFieldInstList.emplace_back(test);
		}
		else if (field.m_fieldType == ScriptFieldType::Int)
		{

      int value = GetFieldValue<int>(field.m_classField);
      ScriptFieldInstance<int> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
		}
		else if (field.m_fieldType == ScriptFieldType::Double)
		{

      double value = GetFieldValue<double>(field.m_classField);
      ScriptFieldInstance<double> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
		}
		else if (field.m_fieldType == ScriptFieldType::DVec3)
		{

      GE::Math::dVec3 value = GetFieldValue<GE::Math::dVec3>(field.m_classField);
      ScriptFieldInstance<GE::Math::dVec3> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
		}
		else if (field.m_fieldType == ScriptFieldType::IntArr)
		{

      std::vector<int> value = GetFieldValueArr<int>(sm->m_appDomain, field.m_classField);
      ScriptFieldInstance<std::vector<int>> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
		}
    else if (field.m_fieldType == ScriptFieldType::UIntArr)
    {

      std::vector<unsigned> value = GetFieldValueArr<unsigned>(sm->m_appDomain, field.m_classField);
      ScriptFieldInstance<std::vector<unsigned>> test{ field,value };
      m_scriptFieldInstList.emplace_back(test);
    }
	}
}


void ScriptInstance::GetAllUpdatedFields()
{
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  ScriptClassInfo sci = sm->GetScriptClassInfo(m_scriptName);
  for (rttr::variant& f: m_scriptFieldInstList)
  {
    rttr::type dataType{ f.get_type() };
    if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<float>>())) 
    {
      GE::MONO::ScriptFieldInstance<float>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<float>>();
      sfi.m_data = GetFieldValue<float>(sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<float>>())) 
    {
      GE::MONO::ScriptFieldInstance<int>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<int>>();
      sfi.m_data = GetFieldValue<int>(sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<float>>())) 
    {
      GE::MONO::ScriptFieldInstance<double>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<double>>();
      sfi.m_data = GetFieldValue<double>(sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<float>>())) 
    {
      GE::MONO::ScriptFieldInstance<GE::Math::dVec3>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<GE::Math::dVec3>>();
      sfi.m_data = GetFieldValue<GE::Math::dVec3>(sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<int>>>())) 
    {
      GE::MONO::ScriptFieldInstance<std::vector<int>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<int>>>();
      sfi.m_data = GetFieldValueArr<int>(sm->m_appDomain,sfi.m_scriptField.m_classField);
    }
    else if ((dataType == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>())) 
    {
      GE::MONO::ScriptFieldInstance<std::vector<unsigned>>& sfi = f.get_value<GE::MONO::ScriptFieldInstance<std::vector<unsigned>>>();
      sfi.m_data = GetFieldValueArr<unsigned>(sm->m_appDomain, sfi.m_scriptField.m_classField);
    }
  }
}




