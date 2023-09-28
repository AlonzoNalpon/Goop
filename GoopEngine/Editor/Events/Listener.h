/*!*********************************************************************
\file   Listener.h
\author chengen.lau\@digipen.edu
\date   27-September-2023
\brief  
  
 
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

      virtual void HandleEvent(Event const* event) = 0;

    protected:
    };
  }
}
