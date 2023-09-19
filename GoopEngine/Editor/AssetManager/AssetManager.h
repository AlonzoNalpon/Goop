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
#include "../Serialization/SpriteGooStream.h"

namespace GE
{
  namespace AssetManager
  {
    enum {
      IMAGES,
      CONFIG,
      ANIMATION,
      AUDIO,
      SCENE,
      OBJECT
    };
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

    // Contains Image Data [ WIP - TO BE GIVEN TO ALONZO ]
    class ImageData
    {
    public:
      /*!*********************************************************************
      \brief
        Basic Constructor
      ************************************************************************/
      ImageData() : m_id(0), m_name(""), m_width(0), m_height(0), m_channels(0), m_data(nullptr) {};

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
      ImageData(int id, std::string name, int width, int height, int channels, unsigned char* data) :
        m_id(id), m_name(name), m_width(width), m_height(height), m_channels(channels), m_data(data) {};

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

      /*!*********************************************************************
      \brief
        Gets the data of the current class object.
      \return
        Data of the loaded image.
      ************************************************************************/
      unsigned char* GetData();

      /*!*********************************************************************
      \brief
        Gets the width of the current class object.
      \return
        Width of the loaded image.
      ************************************************************************/
      unsigned int GetWidth();

      /*!*********************************************************************
      \brief
        Gets the height of the current class object.
      \return
        Height of the loaded image.
      ************************************************************************/
      unsigned int GetHeight();

      unsigned int GetChannels();

      /*!*********************************************************************
      \brief
        Gets the name of the current class object.
      \return
        Name of the loaded image.
      ************************************************************************/
      std::string GetName();

      /*!*********************************************************************
      \brief
        Gets the ID of the current class object.
      \return
        ID of the current class object.
      ************************************************************************/
      int GetID();

    private:
      int m_id; // ID given to the file to be included in the asset manager map.
      std::string m_name; // Name of the file.
      int m_width; // Width of the image file.
      int m_height; // Height of the image file.
      int m_channels; // The amount of channels the image data contains eg. RGB/RGBA
      unsigned char* m_data; // Data of the image.
    };

    // Asset Manager Singleton [ WIP - TO RECEIVE DATA FROM CHENG EN & TO SEND DATA TO ALONZO]
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
      int LoadImage(const std::string& filepath);

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

      /*!*********************************************************************
      \brief
        Loads the json data into the asset manager's map.
      \param
        const std::string& filepath (filepath of the json file)
        int flag (flag for the type of data the json file contains)
      ************************************************************************/
      void LoadJSONData(const std::string& filepath, int flag);

      /*!*********************************************************************
      \brief
        Gets the config data of the specific key.
      \param
        const std::string& key (Key of the config data in the Config.json)
      \return
        Integer data of the specific key
      ************************************************************************/
      int GetConfigData(const std::string& key);

      /*!*********************************************************************
      \brief
        Gets the config data of the specific key.
      \param
        const std::string& key (Key of the config data in the Config.json)
        bool flag (Just to be able to overload)
      \return
        String data of the specific key 
      ************************************************************************/
      const char* GetConfigData(const std::string& key, bool flag);

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

      /*!*********************************************************************
      \brief
        Check if the sprites has been loaded correctly by iterating through
        the m_loadedSpriteData map and printing out the data.
      ************************************************************************/
      void SpriteCheck();

      GE::Serialization::SpriteData GetSpriteData(std::string key);

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
}
#endif