#pragma once
#include "../Debugger/ErrorLogger/ErrorLogger.h"
#include <string>
#ifdef _DEBUG
#include <iostream>
#endif

namespace GE
{
  namespace Events
  {
    class Event
    {
    public:
      Event(std::string&& name) : m_name{ name }
      {
        #ifdef _DEBUG
        std::cout << "Dispatched " + name + " event\n";
        #endif
      }
      virtual inline std::string GetName() const noexcept
      {
        return m_name;
      }

      inline bool IsHandled() const noexcept
      {
        return m_handled;
      }

    protected:
      std::string m_name;
      bool m_handled = false;
    };
  }
}
