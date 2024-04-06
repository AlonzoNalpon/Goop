#pragma once
/*!*********************************************************************
\file   IGooStream.h
\author chengen.lau\@digipen.edu
\date   20-September-2023
\brief  Pure virtual GooStream interface class that provides the class
        with I/O functionality. For serializing GooStreams, inherit
        from IGooIStream (read from file and output into container)
        and for deserializing GooStreams, inherit from IGooOStream
        (read from container and output into file). It is possible to
        inherit from both and acquire the ability to read from a file
        and container.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
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
      /*!*********************************************************************
      \brief
        Reads from a file and stores the raw content in the stream
      \param file
        The name of the file to read from
      \return
        True if the operation succeeded and false otherwise
      ************************************************************************/
      virtual bool Read(std::string const& file) = 0;

      /*!*********************************************************************
      \brief
        Unloads the contents of the stream into a container
      \param container
        The container to output to
      \return
        True if the operation succeeded and false otherwise
      ************************************************************************/
      virtual bool Unload(Type& container) = 0;

    protected:

    };

    template <typename Type>
    class IGooOStream : virtual public GooStream<Type>
    {
    public:
      /*!*********************************************************************
      \brief
        Reads from a container and stores the raw content in the stream
      \param file
        The container to read from
      \return
        True if the operation succeeded and false otherwise
      ************************************************************************/
      virtual bool Read(Type const& container) = 0;

      /*!*********************************************************************
      \brief
        Unloads the contents of the stream into a file
      \param container
        The file to output to
      \return
        True if the operation succeeded and false otherwise
      ************************************************************************/
      virtual bool Unload(std::string const& file, bool overwrite = true) = 0;

    protected:
    };
  }
}