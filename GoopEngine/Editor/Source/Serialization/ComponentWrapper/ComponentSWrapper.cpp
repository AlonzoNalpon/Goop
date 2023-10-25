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

}
