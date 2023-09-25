#pragma once
/*!*********************************************************************
\file   IGooStream.h
\author chengen.lau\@digipen.edu
\date   20-September-2023
\brief


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include "GooStream.h"
#include <string>
#include <rapidjson/ostreamwrapper.h>

namespace GE
{
  namespace Serialization
  {
    template <typename Type>
    class IGooIStream : virtual public GooStream<Type>
    {
    public:
      virtual bool Read(std::string const& file) = 0;
      virtual bool Unload(Type& container) = 0;

    protected:

    };

    template <typename Type>
    class IGooOStream : virtual public GooStream<Type>
    {
    public:
      virtual bool Read(Type const& container) = 0;
      virtual bool Unload(std::string const& file, bool overwrite = true) = 0;

    protected:
    };
  }
}