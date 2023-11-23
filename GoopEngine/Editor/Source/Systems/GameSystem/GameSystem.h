#pragma once
#include <ECS/System/System.h>
#include <Events/Listener.h>

namespace GE::Systems
{
  class GameSystem : public GE::ECS::System, public GE::Events::IEventListener
  {
  public:
    void Update();

    void HandleEvent(GE::Events::Event* event);
  };
}
