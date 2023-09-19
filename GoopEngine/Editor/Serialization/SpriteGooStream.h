/*!*********************************************************************
\file   SpriteGooStream.h
\author chengen.lau\@digipen.edu
\date   18-September-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "GooStream.h"
#include <vector>
#include "SpriteData.h"

namespace GE
{
  namespace Serialization
  {

    // GooStream for Assets
    class SpriteGooStream : public InGooStream<std::vector<SpriteData>>,
                                   TextSerializer
    {
    public:
      // Ctor reading json file into stream
      SpriteGooStream(std::string const& file);

      // Read from a json file. Contents read are appended into GooStream.
      bool Read(std::string const& file) override;

      // Unloads contents into a container
      bool Unload(container_type& container) override;

    private:
      static constexpr char CommentSymbol = '#';
    };

  }
}
