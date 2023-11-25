/*!*********************************************************************
\file   InputEvents.h
\author wchinkit.bryan\@digipen.edu
\date   27-September-2023
\brief  Event subclasses. Currently defined general events.
        class.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "Event.h"

namespace GE
{
  namespace Events
  {
    class StartSceneEvent : public Event
    {
    public:
      StartSceneEvent() : Event(EVENT_TYPE::START_SCENE) {}
      inline std::string GetName() const noexcept override { return "Scene Started"; }
    };

    class PauseSceneEvent : public Event
    {
    public:
      PauseSceneEvent() : Event(EVENT_TYPE::PAUSE_SCENE) {}
      inline std::string GetName() const noexcept override { return "Scene Paused"; }
    };

    class StopSceneEvent : public Event
    {
    public:
      StopSceneEvent() : Event(EVENT_TYPE::STOP_SCENE) {}
      inline std::string GetName() const noexcept override { return "Scene Stopped"; }
    };

    class WindowLoseFocusEvent : public Event
    {
    public:
      WindowLoseFocusEvent() : Event(EVENT_TYPE::WINDOW_LOSE_FOCUS) {}
      inline std::string GetName() const noexcept override { return "Window Lost Focus"; }
    };

    class WindowGainFocusEvent : public Event
    {
    public:
      WindowGainFocusEvent() : Event(EVENT_TYPE::WINDOW_GAIN_FOCUS) {}
      inline std::string GetName() const noexcept override { return "Window Gain Focus"; }
    };

    class GameNextTurn : public Event
    {
    public:
      GameNextTurn() : Event(EVENT_TYPE::NEXT_TURN) {}
      inline std::string GetName() const noexcept override { return "Next Turn"; }
    };

    class GameTurnResolved : public Event
    {
    public:
      GameTurnResolved() : Event(EVENT_TYPE::TURN_RESOLVED) {}
      inline std::string GetName() const noexcept override { return "Turn Resolved"; }
    };

    class QuitGame : public Event
    {
    public:
      QuitGame() : Event(EVENT_TYPE::QUIT_GAME) {}
      inline std::string GetName() const noexcept override { return "Quit Game"; }
    };

    class ToggleFullscreen : public Event
    {
    public:
      ToggleFullscreen() : Event(EVENT_TYPE::TOGGLE_FULLSCREEN) {}
      inline std::string GetName() const noexcept override { return "Toggle Fullscreen"; }
    };
  }
}
