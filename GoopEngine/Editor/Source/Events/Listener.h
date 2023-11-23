/*!*********************************************************************
\file   Listener.h
\author chengen.lau\@digipen.edu
\date   27-September-2023
\brief  Event listener interface to be inherited from for event
        handling. Classes will get access to the HandleEvent function
        that will be called when an event they subscribed to has been
        notified.
  
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "Event.h"

namespace GE
{
  namespace Events
  {
    class IEventListener
    {
    public:
      virtual ~IEventListener() {}

      /*!*********************************************************************
      \brief
        Handles an event that the listener subscribed to
      \param event
        The event to be handled
      ************************************************************************/
      virtual void HandleEvent(Event* event) = 0;

    protected:
    };
  }
}
