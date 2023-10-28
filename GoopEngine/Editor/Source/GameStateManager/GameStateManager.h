/*!*********************************************************************
\file   GameStateManager.h
\author loh.j@digipen.edu
\date   24-October-2023
\brief
  Game State Manager to control the different game states.

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

  public:
    /*!*********************************************************************
    \brief
      Changes scene.
    \param
      std::string (name of the next scene.scn file)
    ************************************************************************/
    void SetNextScene(std::string next_scene);

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

    void Init();
    void Update();
    void Exit();
  };
}