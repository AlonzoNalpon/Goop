/*!*********************************************************************
\file   SpriteGooStream.h
\author chengen.lau\@digipen.edu
\date   18-September-2023
\brief  GooStream class used for deserializing of a text file containing
        sprite animation data. The data of each sprite consists of its
        slices, stacks and frames of the sprite sheet as well as the
        animation speed. The class overrides only input functions.

        For the format of the txt file, you can refer to 
        Assets/Data/Sprites.txt
        For more details on usage, refer to GooStream.h

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "IGooStream.h"
#include <vector>
#include "../SpriteData.h"

namespace GE
{
  namespace Serialization
  {

    // GooStream for Sprites
    class SpriteGooStream : public IGooIStream<std::vector<SpriteData>>
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