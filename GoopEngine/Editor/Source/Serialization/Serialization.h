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


  } // namespace Serialization
} // namespace GE
