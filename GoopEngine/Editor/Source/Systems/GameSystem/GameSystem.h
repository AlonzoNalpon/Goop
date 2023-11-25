#pragma once
#include <ECS/System/System.h>
#include <Events/Listener.h>

namespace GE::Systems
{
  class GameSystem : public GE::ECS::System, public GE::Events::IEventListener
  {
  private:
    bool m_shouldIterate;
    bool m_shouldPause;
    bool m_lastShouldPause;
  public:
    void Start();

    void Update();

    void PlaySoundPlayer(int soundIterator);
    void PlaySoundEnemy(int soundIterator);

    void HandleEvent(GE::Events::Event* event);

    void FlipPauseBool();
  };
}
