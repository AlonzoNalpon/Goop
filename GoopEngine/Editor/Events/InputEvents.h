/*!*********************************************************************
\file   InputEvents.h
\author chengen.lau\@digipen.edu
\date   27-September-2023
\brief  Event subclasses for input. Currently defined for key triggers,
        holds, and releases. They are dispatched from the Input Manager
        class.
  
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
      KeyTriggeredEvent(KEY_CODE code) : Event(EVENT_TYPE::KEY_TRIGGERED), m_code{ code } {}
      inline KEY_CODE GetKey() const noexcept { return m_code; }
      inline std::string GetName() const noexcept override { return "Key Triggered"; }

    private:
      KEY_CODE m_code;
    };

    class KeyHeldEvent : public Event
    {
    public:
      KeyHeldEvent(KEY_CODE code) : Event(EVENT_TYPE::KEY_HELD), m_code{ code } {}
      inline KEY_CODE GetKey() const noexcept { return m_code; }
      inline std::string GetName() const noexcept override { return "Key Held"; }

    private:
      KEY_CODE m_code;
    };

    class KeyReleasedEvent : public Event
    {
    public:
      KeyReleasedEvent(KEY_CODE code) : Event(EVENT_TYPE::KEY_RELEASED), m_code{ code } {}
      inline KEY_CODE GetKey() const noexcept { return m_code; }
      inline std::string GetName() const noexcept override { return "Key Released"; }

    private:
      KEY_CODE m_code;
    };

    class MouseTriggeredEvent : public Event
    {
    public:
      MouseTriggeredEvent(KEY_CODE code) : Event(EVENT_TYPE::MOUSE_TRIGGERED), m_code{ code } {}
      inline KEY_CODE GetKey() const noexcept { return m_code; }
      inline std::string GetName() const noexcept override { return "Mouse Triggered"; }

    private:
      KEY_CODE m_code;
    };

    class MouseHeldEvent : public Event
    {
    public:
      MouseHeldEvent(KEY_CODE code) : Event(EVENT_TYPE::MOUSE_HELD), m_code{ code } {}
      inline KEY_CODE GetKey() const noexcept { return m_code; }
      inline std::string GetName() const noexcept override { return "Mouse Held"; }

    private:
      KEY_CODE m_code;
    };

    class MouseReleasedEvent : public Event
    {
    public:
      MouseReleasedEvent(KEY_CODE code) : Event(EVENT_TYPE::MOUSE_RELEASED), m_code{ code } {}
      inline KEY_CODE GetKey() const noexcept { return m_code; }
      inline std::string GetName() const noexcept override { return "Mouse Released"; }

    private:
      KEY_CODE m_code;
    };
  }
}
