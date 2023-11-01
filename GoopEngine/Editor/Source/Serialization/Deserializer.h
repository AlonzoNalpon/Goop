/*!*********************************************************************
\file   Deserializer.h
\author chengen.lau\@digipen.edu
\date   1-November-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include <string>
#include <Component/ScriptHandler.h>
#include <Component/Sprite.h>
#include <rttr/type.h>
#include <AI/TreeManager.h>

namespace GE
{
  namespace Serialization
  {
    // Static class containing functions related to deserialization
    class Deserializer
    {
    public:
      static std::vector<AI::TreeTemplate> DeserializeTrees(std::string const& filename);

      /*!*********************************************************************
      \brief
        
      \param json
       
      \return
        
      ************************************************************************/
      static rttr::variant DeserializePrefab(std::string const& json);

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
      static std::vector<std::pair<std::string, ECS::ComponentSignature>> DeserializeSystems(std::string const& json);
    private:
      static void DeserializeBasedOnType(rttr::instance object, rapidjson::Value const& value);
      static rttr::variant DeserializeBasicTypes(rapidjson::Value const& value);
      static void DeserializeSequentialContainer(rttr::variant_sequential_view& view, rapidjson::Value const& value);
    };

  } // namespace Serialization
} // namespace GE
