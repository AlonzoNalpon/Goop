#include <pch.h>
#include "Script.h"
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


Script::Script(MonoObject* objectInstance, const std::vector<std::pair<std::string,int>>& allMethodInfo)
{
  m_classObjInst =objectInstance;
  // Get a reference to the method in the class
  MonoClass* instanceClass = mono_object_get_class(objectInstance);
  for (const std::pair<std::string, int>& m : allMethodInfo)
  {
    m_classMethods[m.first] = mono_class_get_method_from_name(instanceClass, m.first.c_str(), m.second);
  }
}


MonoObject* Script::InvokeMethod(const std::string& methodName, void** arg)
{
  return(mono_runtime_invoke(m_classMethods[methodName], m_classObjInst, arg, nullptr));
}





