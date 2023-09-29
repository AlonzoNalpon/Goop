/*!*********************************************************************
\file   AssetGooStream.h
\author chengen.lau\@digipen.edu
\date   18-September-2023
\brief  GooStream class used for serializing / deserializing of an
        asset file (config, images etc.). The json format read by
        this stream must be in key-value pairs ("key": "value").
        It overrides both input and output functions.

        For the format of the json file, you can refer to 
        Assets/Data/Config.json
        For more details on usage, refer to GooStream.h

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "IGooStream.h"
#include <map>

namespace GE
{
  namespace Serialization
  {

    // GooStream for Assets
    class AssetGooStream : public IGooIStream<std::map<std::string, std::string>>,
                                  IGooOStream<std::map<std::string, std::string>>
    {
    public:
      // Ctor reading json file into stream
      AssetGooStream(std::string const& json);

      /*!*********************************************************************
      \brief
        Reads from a json file. Contents read are appended into the stream.
      \param json
        The name of the file to read from
      \return
        True if the operation was successful and false otherwise
      ************************************************************************/
      bool Read(std::string const& json) override;

      /*!*********************************************************************
      \brief
        Reads from a container. Contents read are appended into the stream.
      \param json
        The container to read from
      \return
        True if the operation was successful and false otherwise
      ************************************************************************/
      bool Read(container_type const& container) override;

      /*!*********************************************************************
      \brief
        Unloads the contents of the stream into a container
      \param container
        The container to output into
      \return
        True if the operation was successful and false otherwise
      ************************************************************************/
      bool Unload(container_type& container) override;
      
      /*!*********************************************************************
      \brief
        Unloads the contents of the stream into a file
      \param json
        The file to output into
      \param overwrite
        If true, the file is overwritten with the stream's contents
        If false, the contents are appended to the end of the file
      \return
        True if the operation was successful and false otherwise
      ************************************************************************/
      bool Unload(std::string const& json, bool overwrite = true) override;

    private:
    };

  }
}