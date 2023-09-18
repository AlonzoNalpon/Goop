/*!*********************************************************************
\file   ConfigGooStream.h
\author chengen.lau\@digipen.edu
\date   18-September-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "GooStream.h"
#include <map>

namespace GE
{
  namespace Serialization
  {

    // GooStream for Assets
    class ConfigGooStream : public InGooStream<std::map<std::string,std::string>>, OutGooStream<std::map<std::string,std::string>>
    {
    public:
      // Ctor reading json file into stream
      ConfigGooStream(std::string const& json);

      // Read from a json file. Contents of file are appended into GooStream.
      bool Read(std::string const& json) override;

      // Unloads contents into a AssetGooStream::container_type object
      bool Unload(container_type& container) override;

    private:
      static std::string const ASSETS_DIR;
    };

  }
}
