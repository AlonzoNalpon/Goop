/*!*********************************************************************
\file       SceneManager.h
\author     loh.j\@digipen.edu
\co-author  chengen.lau\@digipen.edu
\date       24-October-2023
\brief
  In charge of:
    - Initializing,
    - Loading,
    - Unloading,
    - Freeing
  scenes to display.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Scenes/Scene.h>
#include <Events/Listener.h>

namespace GE::Scenes
{
  class SceneManager : public Events::IEventListener
  {
  public:
    /*!*********************************************************************
    \brief
      Default Constructor.
    ************************************************************************/
    SceneManager();

    /*!*********************************************************************
    \brief
      Parameterized Constructor.
    ************************************************************************/
    SceneManager(std::string current_scene, std::string next_scene) :
      m_currentScene{ std::move(current_scene) },
      m_nextScene{ std::move(next_scene) } {};

    /*!*********************************************************************
    \brief
      Initialization of the SceneManager. Currently using hardcoded value
      of first scene.
    ************************************************************************/
    void Init();

    /*!*********************************************************************
    \brief
      Calls the load function of the scene class.
    ************************************************************************/
    void LoadScene();

    /*!*********************************************************************
    \brief
      Calls the init function of the scene class.
    ************************************************************************/
    void InitScene();

    /*!*********************************************************************
    \brief
      Switches scene if next scene flag is set.
    ************************************************************************/
    void Update();

    /*!*********************************************************************
    \brief
      Calls the unload function of the scene class.
    ************************************************************************/
    void UnloadScene();

    /*!*********************************************************************
    \brief
      Calls the free function of the scene class.
    ************************************************************************/
    void FreeScene();

    /*!*********************************************************************
    \brief
      Sets the next scene.
    \param
      std::string (name of the next scene.scn file)
    ************************************************************************/
    void SetNextScene(std::string nextScene);

    /*!*********************************************************************
    \brief
      Restarts the current scene by calling the unload then init.
    ************************************************************************/
    void RestartScene();

    /*!*********************************************************************
    \brief
      Handles the events the SceneManager subscribed to

      NEW_SCENE
        - Sets a new empty scene
      START_SCENE
        - Trigger a temporary save before the scene is played in the 
          editor (to later revert to)
      STOP_SCENE
        - Revert to the temporary save when the scene is stopped in
          the editor
      PREFAB_INSTANCES_UPDATED
        - Saves updates to prefab instances to the scene at the top of 
          the m_saves stack
      EDIT_PREFAB
        - Temporarily saves the scene and pushes it to the stack before
          transitioning to prefab editor mode

    \param event
      The event to handle
    ************************************************************************/
    void HandleEvent(Events::Event* event) override;

#ifndef IMGUI_DISABLE
    /*!*********************************************************************
    \brief
      Creates a new scene from a filepath then loads it.
    ************************************************************************/
    void LoadSceneFromExplorer(std::string const& filepath);
#endif

    /*!*********************************************************************
    \brief
      Getter function to get the value of the current scene.
    ************************************************************************/
    std::string GetCurrentScene() const noexcept;

    /*!*********************************************************************
    \brief
      Getter function to get the value of the next scene.
    ************************************************************************/
    std::string GetNextScene() const noexcept;

    /*!*********************************************************************
    \brief
      Performs the necessary operations required to save a scene file
    ************************************************************************/
    void SaveScene() const;

  private:
    std::string m_currentScene;
    std::string m_nextScene;

#ifndef IMGUI_DISABLE

    struct SceneSave
    {
      SceneSave(std::string name, std::string path) : m_name{ std::move(name) }, m_path{ std::move(path) } {}

      std::string const m_name, m_path;
    };

    std::string m_tempPath;
    std::stack<SceneSave> m_tempSaves;  // used to temporarily save scene when playing/stopping

    /*!*********************************************************************
    \brief
      Temporarily saves the scene to m_tempPath and stores the scene name
      in m_tempScene.
    ************************************************************************/
    void TemporarySave();

    /*!*********************************************************************
    \brief
      Loads the scene back from m_tempPath and restores the scene name to
      m_tempScene.
    ************************************************************************/
    void LoadTemporarySave();
#endif

    void InvokeOnCreate() const;

    /* Map of all the scene files.Currently the way scenes are inserted
    into this map is hardcoded. */
    //std::map<std::string, std::unique_ptr<Scene>>Scenes;
    GE::Assets::AssetManager* am = &GE::Assets::AssetManager::GetInstance();
    Scene scene;
  };
}