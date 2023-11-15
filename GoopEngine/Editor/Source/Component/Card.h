#pragma once
#include <ECS/Entity/Entity.h>
#include <ScriptEngine/ScriptManager.h>
#include <Events/Event.h>

namespace GE::Component
{
  class Card
  {
    int m_actionValue;

    GE::MONO::Script m_behaviour;

    // A system will dispatch a card event on card release
    struct CardEvent : public GE::Events::Event
    {
      // Play m_behaviour with value action
      // 
    };

    // A system should do this
    //void Dispatch()
    //{
    //  GE::Events::EventManager::GetInstance().Dispatch()
    //}
  };
}
