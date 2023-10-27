/*!*********************************************************************
\file   AssetStructs.h
\author loh.j@digipen.edu
\date   28 September 2023
\brief
  Contains classes required for AssetManager.cpp.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <pch.h>

namespace GE::Assets
{
  // For generating unique IDs for loading of data.
  class IDGenerator
  {
  public:
    /*!*********************************************************************
    \brief
      Destructor of the IDGenerator class, Clears the unordered set.
    ************************************************************************/
    ~IDGenerator();

    /*!*********************************************************************
    \brief
      Generates a unique ID to be used for loading of assets.
    \return
      ID of the loaded image.
    ************************************************************************/
    int GenerateID();

    /*!*********************************************************************
    \brief
      Adds the free-ed ID to the recycled ID list.
    \param
      int id (ID to be free-ed)
    ************************************************************************/
    void FreeID(int id);

  private:
    int m_nextID = 0; // The next ID to be assigned.
    std::unordered_set<int> m_recycledID; // Set containing free-ed ID of assets.
  };

  /*!*********************************************************************
    \brief
      Constructor which assigns data to the relevant values.
    \param
      int id (ID of the image)
      std::string name (Name of the image)
      int width (Width of the image)
      int height (Height of the image)
      int channels (How many channels the image files contain)
      unsigned char* data (Data of the image file)
  ************************************************************************/
  class ImageData
  {
  public:
    /*!*********************************************************************
    \brief
      Basic constructor
    ************************************************************************/
    ImageData() : m_id(0), m_name(""), m_width(0), m_height(0), m_channels(0), m_data(nullptr) {};

    /*!*********************************************************************
    \brief
      Non-default constructor
    ************************************************************************/
    ImageData(int id, std::string name, int width, int height, int channels, unsigned char* data) :
      m_id(id), m_name(name), m_width(width), m_height(height), m_channels(channels), m_data(data) {};

    /*!*********************************************************************
    \brief
      Null function to return a blank ImageData struct.
    \return
      a blank ImageData struct.
    ************************************************************************/
    static ImageData Null();

    /*!*********************************************************************
    \brief
      Sets the name variable of the ImageData.
    \param
      const std::string& name (New name of the image data)
    ************************************************************************/
    void SetName(const std::string& name);

    /*!*********************************************************************
    \brief
      Sets the relevant information of the ImageData.
    \param
      int width (New width of the image data)
      int height (New height of the image data)
      int channels (New channel of the image data)
      unsigned char* data (New data of the image data)
    ************************************************************************/
    void SetInfo(int width, int height, int channels, unsigned char* data);

    void SetID(unsigned id);

    /*!*********************************************************************
    \brief
      Gets the data of the current class object.
    \return
      Data of the loaded image.
    ************************************************************************/
    unsigned char* GetData()const;

    /*!*********************************************************************
    \brief
      Gets the width of the current class object.
    \return
      Width of the loaded image.
    ************************************************************************/
    unsigned int GetWidth()const;

    /*!*********************************************************************
    \brief
      Gets the height of the current class object.
    \return
      Height of the loaded image.
    ************************************************************************/
    unsigned int GetHeight()const;

    unsigned int GetChannels()const;

    /*!*********************************************************************
    \brief
      Gets the name of the current class object.
    \return
      Name of the loaded image.
    ************************************************************************/
    std::string const& GetName()const;

    /*!*********************************************************************
    \brief
      Gets the ID of the current class object.
    \return
      ID of the current class object.
    ************************************************************************/
    int GetID()const;

  private:
    unsigned m_id; // ID given to the file to be included in the asset manager map.
    std::string m_name; // Name of the file.
    int m_width; // Width of the image file.
    int m_height; // Height of the image file.
    int m_channels; // The amount of channels the image data contains eg. RGB/RGBA
    unsigned char* m_data; // Data of the image.
  };

}