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
#include <ECS/Entity/Entity.h>

namespace GE
{
  namespace Events
  {
#ifndef IMGUI_DISABLE

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

   /* class PrefabSavedEvent : public Event
    {
    public:
      PrefabSavedEvent(std::string prefab) : Event(EVENT_TYPE::PREFAB_SAVED), m_prefab{ std::move(prefab) } {}
      inline std::string GetName() const noexcept override { return "Prefab Saved"; }

      std::string const m_prefab;
    };*/

#endif

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

    class WindowMinimize : public Event
    {
    public:
      WindowMinimize() : Event(EVENT_TYPE::WINDOW_MINIMIZE) {}
      inline std::string GetName() const noexcept override { return "Window Minimize"; }
    };

    //class NewEntityEvent : public Event
    //{
    //public:
    //  NewEntityEvent(ECS::Entity id) : Event(EVENT_TYPE::NEW_ENTITY), m_entityId{ id } {}
    //  inline std::string GetName() const noexcept override { return "Entity " + std::to_string(m_entityId) + " Created"; }

    //  ECS::Entity const m_entityId;
    //};

    class RemoveEntityEvent : public Event
    {
    public:
      RemoveEntityEvent(ECS::Entity id) : Event(EVENT_TYPE::REMOVE_ENTITY), m_entityId{ id } {}
      inline std::string GetName() const noexcept override { return "Entity " + std::to_string(m_entityId) + " Removed"; }

      ECS::Entity const m_entityId;
    };
  }
}
