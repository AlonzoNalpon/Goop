/*!*********************************************************************
\file   EventManager.h
\author chengen.lau\@digipen.edu
\date   27-September-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "../Singleton/Singleton.h"
#include "Listener.h"
#include <unordered_map>
#include <vector>
#include <typeindex>  // for std::type_index
#include <algorithm>

namespace GE
{
  namespace Events
  {
    class EventManager : public Singleton<EventManager>
    {
    public:
      void SubscribeAllListeners();
      template <typename EventType, typename Listener>
      void Subscribe(Listener* listener);  // Subscribes a listener to an event
      template <typename EventType, typename Listener>
      void Unsubscribe(Listener* listener);  // Unsubscribes a listener to an event
      template <typename EventType>
      void Dispatch(EventType&& event); // Dispatches / Triggers an event

    private:
      // alias for function that handles events
      using SubscriberList = std::vector<EventListener*>;
      using SubscriberMap = std::unordered_map<std::type_index, SubscriberList>;

      // list of subscribers for each event
      // each event in the map will have its own list of subscribers
      SubscriberMap m_Subscribers;
    };

    #include "EventManager.tpp"
  }
}
