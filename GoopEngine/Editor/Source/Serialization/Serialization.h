/*!*********************************************************************
\file   Serialization.h
\author chengen.lau\@digipen.edu
\date   15-October-2023
\brief  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include <string>
#include <sstream>
#include <Component/ScriptHandler.h>

namespace GE
{
  namespace Serialization
  {

    void SerializeSystems(std::string const& json);

    /*!*********************************************************************
    \brief
      Reads from a json file of systems and its components and returns
      an std::vector of systems in their order specified together with
      the components to be registered to it

      Each element in the vector is an std::pair containing
      1. The name of the system
      2. The components that should be registered to it
         (in the form of a signature)
    \param json
        The file to read from
    \return
      std::vector of systems to their respective components
    ************************************************************************/
    std::vector<std::pair<std::string, ECS::ComponentSignature>> DeserializeSystems(std::string const& json);

    rapidjson::Value SerializeScriptMap(std::map<std::string, GE::MONO::Script> const& rhs,
      rapidjson::Document::AllocatorType& allocator);

    //rttr::variant DeserializeBasicTypes(rapidjson::Value const& jsonVal)
    //{
    //  switch (jsonVal.GetType())
    //  {
    //  // booleans
    //  case rapidjson::kTrueType:
    //  case rapidjson::kFalseType:
    //    return jsonVal.GetBool();

    //  case rapidjson::kStringType:
    //    return std::string(jsonVal.GetString());

    //  case rapidjson::kNumberType:
    //    if (jsonVal.IsInt())
    //      return jsonVal.GetInt();
    //    else if (jsonVal.IsUint())
    //      return jsonVal.GetUint();
    //    else if (jsonVal.IsDouble())
    //      return jsonVal.GetDouble();
    //    else if (jsonVal.IsFloat())
    //      return jsonVal.GetFloat();
    //    else if (jsonVal.IsInt64())
    //      return jsonVal.GetInt64();
    //    else if (jsonVal.IsUint64())
    //      return jsonVal.GetUint64();
    //    break;
    //  }

    //  return rttr::variant();
    //}

  } // namespace Serialization
} // namespace GE
