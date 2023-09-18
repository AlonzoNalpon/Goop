/*!*********************************************************************
\file   AssetManager.h
\author loh.j@digipen.edu
\date   13-September-2023
\brief
  Asset Manager capable of:
    - LoadDirectory
        Loading all files located in directory

    - LoadImage
        Loading specific image from path

    - GetData [WIP]
        Get image data for alonzo.

    - LoadDeserializedData [WIP]
        Read filepath from deserialized data and load files.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include "../Singleton/Singleton.h"


namespace GE
{
  namespace AssetManager
  {
    // For generating unique IDs for loading of data.
    class IDGenerator
    {
    public:
      int GenerateID();
      void FreeID(int id);

    private:
      int m_next_id = 0;
      std::unordered_set<int> m_recycled_id;
    };

    // Contains Image Data [ WIP - TO BE GIVEN TO ALONZO ]
    class ImageData
    {
    public:
      ImageData() : m_id(0), m_name(""), m_width(0), m_height(0), m_channels(0), m_data(nullptr) {};
      ImageData(int id, std::string name, int width, int height, int channels, unsigned char* data) :
        m_id(id), m_name(name), m_width(width), m_height(height), m_channels(channels), m_data(data) {};

      void SetName(const std::string& name);
      void SetInfo(int width, int height, int channels, unsigned char* data);
      unsigned char* GetData();
      unsigned int GetWidth();
      unsigned int GetHeight();
      unsigned int GetChannels();
      std::string GetName();
      int GetID();

    private:
      int m_id;
      std::string m_name;
      int m_width;
      int m_height;
      int m_channels;
      unsigned char* m_data;
    };

    // Asset Manager Singleton [ WIP - TO RECEIVE DATA FROM CHENG EN & TO SEND DATA TO ALONZO]
    class AssetManager : public Singleton<AssetManager>
    {
    public:

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
        const std::string& path (Filepath of the image)
      \return
        ID of the loaded image.
      ************************************************************************/
      int LoadImage(const std::string& path);

      /*!*********************************************************************
      \brief
        Gets image data to be used in rendering of image.
      \param
        int id (id of image)
      \return
        Image data of file
      ************************************************************************/
      const std::string& GetName(int id);

      /*!*********************************************************************
      \brief
        Gets image data to be used in rendering of image.
      \param
        int id (id of image)
      \return
        Image data of file
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
      ImageData GetData(int id);

      /*!*********************************************************************
      \brief
        Gets image data to be used in rendering of image.
      \param 
        const std::string& name (filepath of image)
      \return
        Image data of file
      ************************************************************************/
      ImageData GetData(const std::string& name);

      void GetDimensions(int id, unsigned int& width, unsigned int& height);

      /*!*********************************************************************
      \brief
        [ WIP ] FOR CHENG EN!
      ************************************************************************/
      std::vector <std::pair<std::string, std::string>> MOCK_Deserialize();

      /*!*********************************************************************
      \brief
        [ WIP ] Load data from deserialized map.
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
      ************************************************************************/
      void FreeImage(const std::string& name);

    private:
      IDGenerator m_generator; // Generates Unique ID to assign to loaded image data.
      std::map<int, ImageData> m_loadedImages; // Map that contains all the loaded images data with an ID as a key.
      std::map<std::string, int> m_loadedImages_string_LU; // Lookup table for getting ID with filepath.
      std::map<int, std::string> m_loadedImages_ID_LU; // Lookup table for getting filepath with id.
    };
  }
}
#endif