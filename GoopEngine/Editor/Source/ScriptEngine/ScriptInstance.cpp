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


ScriptInstance::ScriptInstance(const std::string& scriptName, std::vector<void*>& arg)
{
  GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
  m_monoClass = sm->GetMonoClass(scriptName);
  m_classInst = sm->InstantiateClass(scriptName.c_str(), arg);
  m_onUpdateMethod = mono_class_get_method_from_name(m_monoClass,"OnUpdate", 1);
  m_onCreateMethod = mono_class_get_method_from_name(m_monoClass, "onCreate", 1);
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






