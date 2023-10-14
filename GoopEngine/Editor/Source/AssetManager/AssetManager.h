/*!*********************************************************************
\file   AssetManager.h
\author loh.j@digipen.edu
\date   13-September-2023
\brief
  Asset Manager is capable of:
    - LoadDirectory
        Loading all files located in directory

    - LoadImageW
        Loading specific image from path

    - GetData
        Get image data for graphics.

    - LoadJSONData
        Load Json files and save it into the class.
        Able to load Images, Config, and Animation json.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include <AssetManager/AssetStructs.h>
#include "../Singleton/Singleton.h"
#include "../Serialization/SpriteGooStream.h"

namespace GE::AssetManager
{
  enum {
    IMAGES,
    CONFIG,
    ANIMATION,
    AUDIO,
    SCENE,
    OBJECT
  };
    
  // AssetManager Singleton
  class AssetManager : public Singleton<AssetManager>
  {
  public:

    /*!*********************************************************************
    \brief
      Destructor of the AssetManager class, Frees all loaded images when
      destroyed.
    ************************************************************************/
    ~AssetManager();

    /*!*********************************************************************
    \brief
      Loads all the images in a specific directory and adds them into the map
      of loaded images.
    \param
      onst std::string& directory (Filepath of the directory)
    ************************************************************************/
    void LoadDirectory(const std::string& directory);

    /*!*********************************************************************
    \brief
      Loads the image and add them into the map of loaded images.
    \param
      const std::string& filepath (Filepath of the image)
    \return
      ID of the loaded image.
    ************************************************************************/
    int LoadImageW(const std::string& path);

    /*!*********************************************************************
    \brief
      Gets the key string of the Image in the map from key ID.
    \param
      int id (id of image)
    \return
      Key string of the Image.
    ************************************************************************/
    const std::string& GetName(int id);

    /*!*********************************************************************
    \brief
      Gets the key ID of the Image in the map from key string.
    \param
      const std::string& name (Key string of the image)
    \return
      Key ID of the Image.
    ************************************************************************/
    int GetID(const std::string& name);

    /*!*********************************************************************
    \brief
      Gets image data to be used in rendering of image.
    \param
      int id (id of image)
    \return
      Image data of file
    ************************************************************************/
    ImageData const& GetData(int id);

    /*!*********************************************************************
    \brief
      Gets image data to be used in rendering of image.
    \param 
      const std::string& name (filepath of image)
    \return
      Image data of file
    ************************************************************************/
    ImageData const& GetData(const std::string& name);

    /*!*********************************************************************
    \brief
      Loads the json data into the asset manager's map.
    \param
      const std::string& filepath (filepath of the json file)
      int flag (flag for the type of data the json file contains)
    ************************************************************************/
    void LoadJSONData(const std::string& filepath, int flag);

    /*template <>
    std::optional<double> GetConfigData<double>(const std::string& key) const;
    template <>
    std::optional<const char*> GetConfigData<const char*>(const std::string& key) const;
    template <>
    std::optional<std::string> GetConfigData<std::string>(const std::string& key) const;*/

    /*!*********************************************************************
    \brief
      Gets dimensions of the data.
    \param
      int id (ID for the image data in the map)
      unsigned int& width (Receives a pointer to be assigned the width)
      unsigned int& height (Receives a pointer to be assigned the height)
    ************************************************************************/
    void GetDimensions(int id, unsigned int& width, unsigned int& height);

    /*!*********************************************************************
    \brief
      Hard coded test for receiving deserialized data.
    \return
      vector of key and data pair.
    ************************************************************************/
    std::vector <std::pair<std::string, std::string>> MOCK_Deserialize();

    /*!*********************************************************************
    \brief
      Load data from deserialized map.
    ************************************************************************/
    void LoadDeserializedData();

    /*!*********************************************************************
    \brief
      Free all loaded images.
    ************************************************************************/
    void FreeImages();

    /*!*********************************************************************
    \brief
      Free specific loaded images with filepath.
    \param
      const std::string& name (Name of the image)
    ************************************************************************/
    void FreeImage(const std::string& name);

    /*!*********************************************************************
    \brief
      Check if the sprites has been loaded correctly by iterating through
      the m_loadedSpriteData map and printing out the data.
    ************************************************************************/
    void SpriteCheck();

    /*!*********************************************************************
    \brief
      Loads the required files for startup.
    ************************************************************************/
    void LoadFiles();

    /*!*********************************************************************
    \brief
      Loads the required files for startup.
    \param
      std::string key (Key of the data)
    \return
      SpriteData for graphics
    ************************************************************************/
    GE::Serialization::SpriteData GetSpriteData(std::string key);

    #include "AssetManager.tpp"

  private:
    IDGenerator m_generator; // Generates Unique ID to assign to loaded image data.
    std::map<std::string, std::string> m_filePath; // name:filepath
    std::map<std::string, std::string> m_configData;
    std::map<int, ImageData> m_loadedImages; // Map that contains all the loaded images data with an ID as a key.
    std::map<std::string, int> m_loadedImagesStringLookUp; // Lookup table for getting ID with filepath.
    std::map<int, std::string> m_loadedImagesIDLookUp; // Lookup table for getting filepath with id.
    std::map<std::string, GE::Serialization::SpriteData> m_loadedSpriteData; // Map that contains loaded sprite with their data with their m_id as key.

  };
}
#endif