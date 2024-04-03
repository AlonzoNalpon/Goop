/*!*********************************************************************
\file   ProxyScripts.h
\author chengen.lau\@digipen.edu
\date   13-March-2024
\brief  Struct used to hold raw json data of scripts. It acts as an
        intermediary object to handle script components differently
        from the rest during deserialization. It will only be
        deserialized to create a script instance when the component
        is added.
  
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <string>
#include <Component/Scripts.h>
#include <rapidjson/document.h>

namespace GE::Serialization
{

  struct ProxyScripts
  {
    std::string m_scriptData;

    ProxyScripts(rapidjson::Value const& data);
  };

} // namespace GE::Serialization
