/*!*********************************************************************
\file   GameSystem.h
\author w.chinkitbryan\@digipen.edu
\date   26-November-2023
\brief
  Single point interface where all our game logic will go through.
  A entity with the Game component with a GameManager.cs script will
  run and call all our game logic in 1 script which will then handle
  logic in scripts entirely enclosed in the C# enviroment.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <ECS/System/System.h>
#include <Events/Listener.h>

namespace GE::Systems
{
  class GameSystem : public GE::ECS::System, public GE::Events::IEventListener
  {
  private:
    bool m_shouldIterate;
    bool m_shouldPause;
    bool m_lastShouldPause;

    bool m_shouldWin;
    bool m_shouldLose;
  public:
    /*!*********************************************************************
    \brief
      System Start
    ************************************************************************/
    void Start();

    /*!*********************************************************************
    \brief
      System Update. Will process the 1 entity with GameManager
    ************************************************************************/
    void Update();

    /*!*********************************************************************
    \brief
      Handles event dispatched by the event manager
    \param event
      Event to handle
    ************************************************************************/
    void HandleEvent(GE::Events::Event* event);

    /*!*********************************************************************
    \brief
      Flips the system should pause bool
    ************************************************************************/
    void FlipPauseBool();
  };
}
