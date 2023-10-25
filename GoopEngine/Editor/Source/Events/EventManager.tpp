template <typename EventType, typename Listener>
void EventManager::Subscribe(Listener* listener)
{
  SubscriberList& subscribers{ m_Subscribers[typeid(EventType)] };
  // if already in list, return

  if (std::find(subscribers.begin(), subscribers.end(), listener) != subscribers.end())
  {
    #ifdef _DEBUG
    std::cout << "EventManager::Subscribe: " << typeid(Listener).name() 
    << " already subscribed to " << typeid(EventType).name() << "\n";
    #endif

    return;
  }

  subscribers.emplace_back(static_cast<IEventListener*>(listener));

  #ifdef _DEBUG
  std::ostringstream oss{};
  oss << "EventManager: " << typeid(EventType).name() 
  << " subscribed to " << typeid(EventType).name() << "\n";
  Debug::ErrorLogger::GetInstance().LogMessage(oss.str());
  #endif
}

template <typename EventType, typename Listener>
void EventManager::Unsubscribe(Listener* listener)
{
  SubscriberList& subscribers{ m_Subscribers[typeid(EventType)] };
  typename SubscriberList::const_iterator result{ std::find(subscribers.cbegin(), subscribers.cend(), listener) };
  if (result == subscribers.end())
  {
    #ifdef _DEBUG
    std::ostringstream oss{};
    oss << "EventManager::Unsubscribe: " << typeid(Listener).name()
    << " is not currently subscribed to " << typeid(EventType).name() << "\n";
    Debug::ErrorLogger::GetInstance().LogMessage(oss.str());
    #endif

    return;
  }

  subscribers.erase(result);

  #ifdef _DEBUG
  std::ostringstream oss{};
  oss << "EventManager: " << typeid(Listener).name() << 
  " unsubscribed from " << typeid(EventType).name() << "\n";
  Debug::ErrorLogger::GetInstance().LogMessage(oss.str());
  #endif
}

template <typename EventType>
    void EventManager::Dispatch(EventType const& event)
    {
      SubscriberList& subscribers{ m_Subscribers[typeid(EventType)] };
      for (SubscriberList::iterator it{ subscribers.begin() }; it != subscribers.end();)
      {
        if (event.IsHandled()) { return; }
        // if listener being pointed to no longer exists, remove it
        if (!(*it))
        {
          #ifdef _DEBUG
          std::cout << "EventManager::Dispatch: Invalid pointer found and removed\n";
          #endif

          it = subscribers.erase(it);
          continue;
        }

        (*it)->HandleEvent(static_cast<const Event*>(&event));
        ++it;
      }
    }