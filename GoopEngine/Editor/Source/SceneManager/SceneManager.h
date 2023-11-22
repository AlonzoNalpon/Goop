/*!*********************************************************************
\file   SceneManager.h
\author loh.j@digipen.edu
\date   24-October-2023
\brief
  In charge of:
    - Initializing,
    - Loading,
    - Unloading,
    - Freeing
  scenes to display.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
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

    void HandleEvent(Events::Event* event) override;

    /*!*********************************************************************
    \brief
      Creates a new scene from a filepath then loads it.
    ************************************************************************/
    void LoadSceneFromExplorer(std::string const& filepath);

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
    std::string m_tempScene;  // used to temporarily save scene when playing/stopping

    void TemporarySave() const;

    void LoadTemporarySave();

    /* Map of all the scene files.Currently the way scenes are inserted
    into this map is hardcoded. */
    //std::map<std::string, std::unique_ptr<Scene>>Scenes;
    GE::Assets::AssetManager* am = &GE::Assets::AssetManager::GetInstance();
    Scene scene;
  };
}