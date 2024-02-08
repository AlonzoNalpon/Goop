/*!*********************************************************************
\file   AssetManager.h
\author loh.j\@digipen.edu
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
#include <Singleton/Singleton.h>
#include <Serialization/GooStream/SpriteGooStream.h>
#include <DebugTools/Exception/Exception.h>
#include "AssetTypes.h"
#ifndef IMGUI_DISABLE
#include <Events/Listener.h>
#endif

namespace GE::Assets
{    
  // AssetManager Singleton
  class AssetManager : public Singleton<AssetManager>
#ifndef IMGUI_DISABLE
    , public Events::IEventListener
#endif
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
      Extracts the filename of the path without '/' and '.XXX'.
    \param
      const std::string& (filepath)
    ************************************************************************/
    std::string ExtractFilename(const std::string& filepath) const;

    /*!*********************************************************************
    \brief
      Loads all the images in a specific directory and adds them into the map
      of loaded images.
    \param
      const std::string& directory (Filepath of the directory)
    ************************************************************************/
    std::set<int> LoadDirectory(const std::string& directory);

    /*!*********************************************************************
    \brief
      Checks if the filepath is already loaded.
    \param
      const std::string& directory (Filepath of the directory)
    \return
      true if the file is already loaded.
    ************************************************************************/
    bool AlreadyLoaded(const std::string& path) const;

    /*!*********************************************************************
    \brief
      Checks id of texture is already loaded
    \param
      const std::string& directory (Filepath of the directory)
    \return
      true if the id is already loaded.
    ************************************************************************/
    bool AlreadyLoaded(int id) const;

    /*!*********************************************************************
    \brief
      Loads the image and add them into the map of loaded images.
    \param
      const std::string& filepath (Filepath of the image)
    \return
      ID of the loaded image.
    ************************************************************************/
    unsigned LoadImageW(const std::string& path);

    /*!*********************************************************************
    \brief
      Clears the asset manager's config map.
    ************************************************************************/
    void ClearConfigData();

    /*!*********************************************************************
    \brief
      Loads the json data into the asset manager's map.
    \param
      const std::string& filepath (filepath of the json file)
      int flag (flag for the type of data the json file contains)
    ************************************************************************/
    void LoadConfigData(const std::string& filepath);

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
      Checks whether a particular scene exists in the Scenes directory
    \param sceneName
      The name of the scene
    \return
      True if the scene exists and false otherwise
    ************************************************************************/
    inline bool HasScene(std::string const& sceneName) const noexcept
    { return m_scenes.find(sceneName) != m_scenes.cend(); }

    /*!*********************************************************************
    \brief
      Gets the filepath of a scene based on the specified name. Returns an
      empty string if the entry doesn't exist
    \param sceneName
      The key or name of the scene
    \return
      The filepath of the scene and an empty string otherwise
    ************************************************************************/
    std::string GetScene(std::string const& sceneName);

    /*!*********************************************************************
    \brief
      Gets the filepath of an audio file based on the specified name
      (may need to make this function safer in future)
    \param soundName
      The key or name of the scene
    \return
      The value at the entry of the specified key
    ************************************************************************/
    std::string GetSound(std::string const& soundName);

    /*!*********************************************************************
    \brief
      Gets image data to be used in rendering of image.
    \param
      int id (id of image)
    \return
      Image data of file
    ************************************************************************/
    ImageData const& GetData(int id) const;

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
      Gets dimensions of the data.
    \param
      int id (ID for the image data in the map)
      unsigned int& width (Receives a pointer to be assigned the width)
      unsigned int& height (Receives a pointer to be assigned the height)
    ************************************************************************/
    void GetDimensions(int id, unsigned int& width, unsigned int& height);

    /*!*********************************************************************
    \brief
      Free all loaded images.
    ************************************************************************/
    void FreeImages();

    /*!*********************************************************************
    \brief
      Free specific loaded images with name of file.
    \param
      const std::string& name (Name of the image)
    ************************************************************************/
    void FreeImage(const std::string& name);

    /*!*********************************************************************
    \brief
      Free specific loaded images with id.
    \param
      const std::string& name (Name of the image)
    ************************************************************************/
    void FreeImage(int id);


    /*!*********************************************************************
    \brief
      For Debugging purposes. Prints to  all the items in the map.
    ************************************************************************/
    void GetMapData();

    /*!*********************************************************************
    \brief
      Check if the sprites has been loaded correctly by iterating through
      the m_loadedSpriteData map and printing out the data.
    ************************************************************************/
    void SpriteCheck();

    /*!*********************************************************************
    \brief
      Loads the fonts.
    ************************************************************************/
    void LoadFonts();

    /*!*********************************************************************
    \brief
      Loads the required files for startup.
    ************************************************************************/
    void LoadFiles();

    /*!*********************************************************************
    \brief
      Reloads files by scanning the Assets directory
    ************************************************************************/
    void ReloadFiles(Assets::AssetType type);

    /*!*********************************************************************
    \brief
      Reloads all files by scanning the Assets directory
    ************************************************************************/
    void ReloadAllFiles();

    /*!*********************************************************************
    \brief
      Loads the required files for startup.
    \param
      std::string key (Key of the data)
    \return
      SpriteData for graphics
    ************************************************************************/
    GE::Serialization::SpriteData GetSpriteData(std::string const& key);

    /*!*********************************************************************
    \brief
      Returns the map of prefab filepaths as a const reference
    \return
      The map of prefab filepaths
    ************************************************************************/
    inline std::unordered_map<std::string, std::string> const& GetPrefabs() const noexcept { return m_prefabs; }

    inline std::unordered_map<std::string, std::string> const& GetImages() const noexcept { return m_images; }

    inline std::unordered_map<std::string, std::string> const& GetScenes() const noexcept { return m_scenes; }

#ifndef IMGUI_DISABLE
    /*!*********************************************************************
    \brief
      Handles the events the AssetManager subscribed to

      NEW_SCENE
        - Creates a new scene file and loads it into m_scenes
      DELETE_ASSET
        - Clears the respective entry from the map

    \param event
      The event to handle
    ************************************************************************/
    void HandleEvent(Events::Event* event) override;
#endif

#include "AssetManager.tpp"

    std::string AudioFileExt, ImageFileExt, ShaderFileExt, FontFileExt, SceneFileExt, PrefabFileExt;
  private:
    /*!*********************************************************************
    \brief
      Load data from deserialized map.
    ************************************************************************/
    void LoadImages();

    /*!*********************************************************************
    \brief
      Load data required for each spritesheet (stacks, slices etc.)
    ************************************************************************/
    void LoadSpritesheets();

    // IDGenerator m_generator; // Generates Unique ID to assign to loaded image data.

    // maps storing each type of file with format <name : filepath>
    std::unordered_map<std::string, std::string> m_images;
    std::unordered_map<std::string, std::string> m_audio;
    std::unordered_map<std::string, std::string> m_prefabs;
    std::unordered_map<std::string, std::string> m_scenes;
    std::unordered_map<std::string, std::string> m_shaders;
    std::unordered_map<std::string, std::string> m_configData;
    std::unordered_map<std::string, std::string> m_fonts;

    std::unordered_map<int, ImageData> m_loadedImages; // Map that contains all the loaded images data with an ID as a key.
    std::unordered_map<std::string, int> m_loadedImagesStringLookUp; // Lookup table for getting ID with filepath.
    std::unordered_map<int, std::string> m_loadedImagesIDLookUp; // Lookup table for getting filepath with id.
    std::unordered_map<std::string, GE::Serialization::SpriteData> m_loadedSpriteData; // Map that contains loaded sprite with their data with their m_id as key.
  };
}
#endif