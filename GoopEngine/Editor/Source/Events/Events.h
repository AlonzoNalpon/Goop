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
    class WindowLoseFocusEvent : public Event
    {
    public:
      WindowLoseFocusEvent() : Event(EVENT_TYPE::WINDOW_LOSE_FOCUS) {}
      inline std::string GetName() const noexcept override { return "Window Lost Focus"; }
    };
  }
}
