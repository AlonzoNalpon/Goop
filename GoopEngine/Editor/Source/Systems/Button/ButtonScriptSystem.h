/*!*********************************************************************
\file   ButtonScriptSystem.h
\author w.chinkitbryan\@digipen.edu
\date   16-December-2023
\brief  
  This systems calls the appropriate script of behaviour for the
  appropriate entity  
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <ECS/EntityComponentSystem.h>
#include <Events/Events.h>

namespace GE::Systems
{
  class ButtonScriptSystem : public GE::ECS::System, public GE::Events::IEventListener
  {
  private:
    bool m_shouldHandleClick;
    bool m_shouldHandleRelease;

  public:
    /*!*********************************************************************
    \brief
      Starts the system and subscribes to events
    ************************************************************************/
    void Start();

    /*!*********************************************************************
    \brief
      Update function of the system. Checks if a button was selected on
      and runs the script attached to it.
    ************************************************************************/
    void Update();

    /*!*********************************************************************
    \brief
      Handles the events the class subscribed to
    \param event
      The event to handle
    ************************************************************************/
    void HandleEvent(GE::Events::Event* event);
  };
}
