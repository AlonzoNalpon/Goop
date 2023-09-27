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
      MAX_EVENTS
    };

    class Event
    {
    public:
      Event(EVENT_TYPE type) : m_category{ type }
      {
        #ifdef _DEBUG
        std::cout << "Dispatched " + GetName() + " event\n";
        #endif
      }
      virtual inline std::string GetName() const noexcept { return "Base Event"; }

      inline EVENT_TYPE GetCategory() const noexcept { return m_category; }

      inline bool IsHandled() const noexcept { return m_handled; }

    protected:
      EVENT_TYPE m_category;
      bool m_handled = false;
    };
  }
}
