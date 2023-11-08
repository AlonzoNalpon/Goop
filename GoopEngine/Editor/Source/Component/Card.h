#pragma once
#include <ECS/Entity/Entity.h>
#include <ScriptEngine/ScriptManager.h>
#include <Events/Event.h>

namespace GE::Component
{
  class Card
  {
    int m_actionValue;
    GE::ECS::Entity m_source;
    GE::ECS::Entity m_target;

    GE::MONO::Script m_behaviour;

    struct CardEvent : public GE::Events::Event
    {

    };

    void Dispatch()
    {
      //GE::Events::EventManager::GetInstance().Dispatch()
    }
  };
}
