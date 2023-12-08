/*!*********************************************************************
\file   Event.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Base event class for events/messaging system. All events inherit
        from base class Event and will by default contain a name and a 
        category. Each individual event will hold its own set of 
        variables that will be passed to the listener when handling 
        the event. (E.g. input events hold the key code for the key 
        that was pressed)
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <DebugTools/ErrorLogger/ErrorLogger.h>
#include <string>
#ifdef _DEBUG
#include <iostream>
#endif

namespace GE
{
  namespace Events
  {
    enum class EVENT_TYPE : unsigned
    {
      KEY_TRIGGERED = 0,
      KEY_HELD,
      KEY_RELEASED,
      MOUSE_TRIGGERED,
      MOUSE_HELD,
      MOUSE_RELEASED,
      WINDOW_LOSE_FOCUS,
      WINDOW_GAIN_FOCUS,
      WINDOW_MINIMIZE,
      TOGGLE_FULLSCREEN,
      QUIT_GAME,
      NEXT_TURN,
      TURN_RESOLVED,
      NEW_ENTITY,
      REMOVE_ENTITY,

#ifndef IMGUI_DISABLE
      START_SCENE,
      PAUSE_SCENE,
      STOP_SCENE,
      PREFAB_SAVED,
#endif
      MAX_EVENTS
    };

    class Event
    {
    public:
      Event(EVENT_TYPE type) : m_category{ type } {}
      
      /*!*********************************************************************
      \brief
        Returns the name of the current event
      \return
        The name of the current event
      ************************************************************************/
      virtual inline std::string GetName() const noexcept { return "Base Event"; }

      /*!*********************************************************************
      \brief
        Returns the category of the current event
      \return
        The category of the current event
      ************************************************************************/
      inline EVENT_TYPE GetCategory() const noexcept { return m_category; }

      /*!*********************************************************************
      \brief
        Returns the status of the current event
      \return
        True if the event has already been handled and false otherwise
      ************************************************************************/
      inline bool IsHandled() const noexcept { return m_handled; }

      /*!*********************************************************************
      \brief
        Virtual destructor.
      ************************************************************************/
      virtual ~Event() {}

    protected:
      EVENT_TYPE m_category;
      bool m_handled = false;
    };
  }
}
