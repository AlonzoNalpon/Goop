/*!*********************************************************************
\file   ProxyScripts.cpp
\author chengen.lau\@digipen.edu
\date   13-March-2024
\brief  Struct used to hold raw json data of scripts. It acts as an
        intermediary object to handle script components differently
        from the rest during deserialization. It will only be
        deserialized to create a script instance when the component
        is added.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ProxyScripts.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <Serialization/Serializer.h>

namespace GE::Serialization
{
  ProxyScripts::ProxyScripts(rapidjson::Value const& data)
  {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    data.Accept(writer);

    m_scriptData = buffer.GetString();
  }

  //ProxyScripts::ProxyScripts(Component::Scripts const& scripts)
  //{
  //  rapidjson::Document document;
  //  rapidjson::Value data{ Serialization::Serializer::SerializeComponent(scripts, document.GetAllocator()) };

  //  rapidjson::StringBuffer buffer;
  //  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  //  data.Accept(writer);

  //  m_scriptData = buffer.GetString();
  //}
} // namespace GE::Serialization