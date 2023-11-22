/*!*********************************************************************
\file   ObjectGooStream.h
\author chengen.lau\@digipen.edu
\date   22-September-2023
\brief  GooStream class used for serializing / deserializing of a file
        of multiple entities (scene file), together with its individual
        components and their values. The class overrides both input
        and output functions.

        For the format of the json file, you can refer to
        Assets/Data/Scene.json
        For more details on usage, refer to GooStream.h

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "IGooStream.h"
#include <ObjectFactory/ObjectStructs.h>

#ifndef RTTR_DESERIALIZE
namespace GE
{
  namespace Serialization
  {

    // GooStream for Entities/Objects
    class ObjectGooStream : public IGooIStream<std::vector<std::pair<ECS::Entity, ObjectFactory::ObjectData>>>
    {
    public:
      // Ctor reading json file into stream
      ObjectGooStream(std::string const& json);

      /*!*********************************************************************
      \brief
        Read from a json file. Contents read are appended into the stream.
        This function will check for the formatting for the file when reading
        its contents. It ensures that the data is nested properly and that
        each object contains the correct keys.
      \param json
        The file to read from
      \return
        True if the operation succeeded and false otherwise
      ************************************************************************/
      bool Read(std::string const& json) override;

      /*!*********************************************************************
      \brief
        Unloads the contents of the stream into a container
      \param json
        The container to output to
      \return
        True if the operation succeeded and false otherwise
      ************************************************************************/
      bool Unload(container_type& container) override;

    private:
    };

  }
}
#endif