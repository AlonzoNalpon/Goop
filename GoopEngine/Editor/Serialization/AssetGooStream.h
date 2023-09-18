/*!*********************************************************************
\file   AssetGooStream.h
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
    class AssetGooStream : public InGooStream<std::map<std::string,std::string>>,
                                  OutGooStream<std::map<std::string,std::string>>
    {
    public:
      // Ctor reading json file into stream
      AssetGooStream(std::string const& json);

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
