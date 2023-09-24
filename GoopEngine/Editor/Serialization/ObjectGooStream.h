/*!*********************************************************************
\file   ObjectGooStream.h
\author chengen.lau\@digipen.edu
\date   22-September-2023
\brief


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "IGooStream.h"
#include "Serializer.h"
#include <map>
#include "../ObjectFactory/ObjectFactory.h"

namespace GE
{
  namespace Serialization
  {

    // GooStream for Assets
    class ObjectGooStream : public IGooIStream<std::map<std::string, ObjectFactory::ObjectData>>,
      IGooOStream<std::map<std::string, ObjectFactory::ObjectData>>
    {
    public:
      // Ctor reading json file into stream
      ObjectGooStream(std::string const& json);

      // Read from a json file. Contents read are appended into GooStream.
      bool Read(std::string const& json) override;
      // Read from a constructor. Contents read are appended into GooStream.
      bool Read(container_type const& container) override;

      // Unloads contents into a container
      bool Unload(container_type& container) override;
      // Unloads contents into a file
      bool Unload(std::string const& json, bool overwrite = true) override;

    private:

    };

  }
}