/*!*********************************************************************
\file   InputEvents.h
\author chengen.lau\@digipen.edu
\date   27-September-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "Event.h"
#include "../InputManager/KeyCode.h"

namespace GE
{
  namespace Events
  {
    class KeyTriggeredEvent : public Event
    {
    public:
      KeyTriggeredEvent(KEY_CODE code) : Event("KeyTriggered"), m_code{ code } {}
      inline KEY_CODE GetKey() const noexcept { return m_code; }

    private:
      KEY_CODE m_code;
    };

    class KeyHeldEvent : public Event
    {
    public:
      KeyHeldEvent(KEY_CODE code) : Event("KeyHeld"), m_code{ code } {}
      inline KEY_CODE GetKey() const noexcept { return m_code; }

    private:
      KEY_CODE m_code;
    };

    class KeyReleasedEvent : public Event
    {
    public:
      KeyReleasedEvent(KEY_CODE code) : Event("KeyReleased"), m_code{ code } {}
      inline KEY_CODE GetKey() const noexcept { return m_code; }

    private:
      KEY_CODE m_code;
    };

    class MouseTriggeredEvent : public Event
    {
    public:
      MouseTriggeredEvent(KEY_CODE code) : Event("MouseTriggered"), m_code{ code } {}
      inline KEY_CODE GetKey() const noexcept { return m_code; }

    private:
      KEY_CODE m_code;
    };

    class MouseHeldEvent : public Event
    {
    public:
      MouseHeldEvent(KEY_CODE code) : Event("MouseHeld"), m_code{ code } {}
      inline KEY_CODE GetKey() const noexcept { return m_code; }

    private:
      KEY_CODE m_code;
    };

    class MouseReleasedEvent : public Event
    {
    public:
      MouseReleasedEvent(KEY_CODE code) : Event("MouseReleased"), m_code{ code } {}
      inline KEY_CODE GetKey() const noexcept { return m_code; }

    private:
      KEY_CODE m_code;
    };
  }
}
