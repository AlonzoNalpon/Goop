/*!*********************************************************************
\file   ButtonScriptSystem.h
\author w.chinkitbryan\@digipen.edu
\date   16-December-2023
\brief  
  This systems calls the appropriate script of behaviour for the
  appropriate entity  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
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
    void Start();

    void Update();

    void HandleEvent(GE::Events::Event* event);
  };
}
