/*!*********************************************************************
\file   GameStateManager.h
\author loh.j@digipen.edu
\date   24-October-2023
\brief
  Currently controls the scenes but will include gamestates in the 
	future.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Singleton/Singleton.h>
#include <SceneManager/SceneManager.h>

namespace GE::GSM
{
  class GameStateManager : public Singleton<GameStateManager>
  {
  private:
    GE::Scenes::SceneManager sm;
    bool m_updated{ false };

  public:
    /*!*********************************************************************
    \brief
      Changes scene.
    \param
      std::string (name of the next scene.scn file)
    ************************************************************************/
    void SetNextScene(std::string next_scene);

    /*!*********************************************************************
    \brief
      Gets the current scene.
    \return
      String of the current scene
    ************************************************************************/
    std::string GetCurrentScene() const;

    /*!*********************************************************************
    \brief
      Calls on the scene manager to save the scene
    ************************************************************************/
    void SaveScene() const;

    /*!*********************************************************************
    \brief
      Restarts scene.
    ************************************************************************/
    void Restart();

#ifndef IMGUI_DISABLE
    /*!*********************************************************************
    \brief
      Loads a scene given a file path
    ************************************************************************/
    void LoadSceneFromExplorer(std::string const& filepath);
#endif

    /*!*********************************************************************
    \brief
      Initializes all the required singletons and loads all the data.
    ************************************************************************/
    void Init();

    /*!*********************************************************************
    \brief
      Updates all the data and render the changes.
    ************************************************************************/
    void Update();

    /*!*********************************************************************
    \brief
      Clears all the memory cleanly.
    ************************************************************************/
    void Exit();
  };
}