/*!*********************************************************************
\file   EventManager.h
\author chengen.lau\@digipen.edu
\date   27-September-2023
\brief  The events system is managed by the EventManager singleton. It 
        provides functions to subscribe / unsubscribe listeners to 
        events and is the source of dispatching events. The 
        EventManager holds the map of events to listeners and notifies
        all relevant parties subscribed to a particular event when it
        is dispatched.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Singleton/Singleton.h>
#include "Listener.h"
#include <unordered_map>
#include <vector>
#include <typeindex>  // for std::type_index
#include <algorithm>

namespace GE
{
  namespace Events
  {
    enum class PRIORITY
    {
      HIGH = 0,
      MEDIUM,
      LOW
    };

    struct SubscriberList
    {
      std::vector<IEventListener*> m_listeners;
      std::vector<IEventListener*>::size_type m_medPriorityIndex = 0;  // index at which MEDIUM priority begins
    };

    class EventManager : public Singleton<EventManager>
    {
    public:
      /*!*********************************************************************
      \brief
        Init function to subscribe listeners to their respective events
      ************************************************************************/
      void SubscribeAllListeners();


      /*!*********************************************************************
      \brief
        Subscribes a listener to an event category
      \param listener
        The listener to subscribe to the event
      ************************************************************************/
      template <typename EventType, typename Listener>
      void Subscribe(Listener* listener, PRIORITY priority = PRIORITY::LOW);  // Subscribes a listener to an event

      /*!*********************************************************************
      \brief
        Unsubscribes a listener to an event category
      \param listener
        The listener to unsubscribe from the event
      ************************************************************************/
      template <typename EventType, typename Listener>
      void Unsubscribe(Listener* listener);  // Unsubscribes a listener from an event

      /*!*********************************************************************
      \brief
        Dispatches an event to the respective listeners based on the
        subscribers list
      \param event
        The event to dispatch
      ************************************************************************/
      template <typename EventType>
      void Dispatch(EventType& event); // Dispatches / Triggers an event
      template <typename EventType>
      void Dispatch(EventType&& event);

    private:
      using SubscriberMap = std::unordered_map<std::type_index, SubscriberList>;

      // list of subscribers for each event
      // each event in the map will have its own list of subscribers
      SubscriberMap m_subscribers;
    };

    #include "EventManager.tpp"
  }
}
