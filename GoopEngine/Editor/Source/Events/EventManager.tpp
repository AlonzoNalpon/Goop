template <typename EventType, typename Listener>
void EventManager::Subscribe(Listener* listener, PRIORITY priority)
{
  SubscriberList& sList{ m_subscribers[typeid(EventType)] };
  // if already in list, return

  if (std::find(sList.m_listeners.begin(), sList.m_listeners.end(), listener) != sList.m_listeners.end())
  {
    return;
  }

  // insert based on priority
  switch (priority)
  {
  case PRIORITY::HIGH:
    sList.m_listeners.emplace(sList.m_listeners.begin(), static_cast<IEventListener*>(listener));
    ++sList.m_medPriorityIndex;
    break;
  case PRIORITY::MEDIUM:
    sList.m_listeners.emplace(sList.m_listeners.begin() + sList.m_medPriorityIndex, static_cast<IEventListener*>(listener));
    break;
  case PRIORITY::LOW:
    sList.m_listeners.emplace_back(static_cast<IEventListener*>(listener));
    break;
  }

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
  SubscriberList& sList{ m_subscribers[typeid(EventType)] };
  std::vector<IEventListener*>::const_iterator result{ std::find(sList.m_listeners.cbegin(), sList.m_listeners.cend(), listener) };
  if (result == sList.m_listeners.cend())
  {
    #ifdef _DEBUG
    std::ostringstream oss{};
    oss << "EventManager::Unsubscribe: " << typeid(Listener).name()
        << " is not currently subscribed to " << typeid(EventType).name() << "\n";
    Debug::ErrorLogger::GetInstance().LogMessage(oss.str());
    #endif

    return;
  }

  // if removing high priority listener, decrement medium index
  if (sList.m_medPriorityIndex != 0 && result <= sList.m_listeners.cbegin() + sList.m_medPriorityIndex)
  {
    --sList.m_medPriorityIndex;
  }
  sList.m_listeners.erase(result);

  #ifdef _DEBUG
  std::ostringstream oss{};
  oss << "EventManager: " << typeid(Listener).name() << " unsubscribed from " << typeid(EventType).name() << "\n";
  Debug::ErrorLogger::GetInstance().LogMessage(oss.str());
  #endif
}

template <typename EventType>
void EventManager::Dispatch(EventType& event)
{
  std::vector<IEventListener*>& subscribers{ m_subscribers[typeid(EventType)].m_listeners };
  for (std::vector<IEventListener*>::iterator it{ subscribers.begin() }; it != subscribers.end();)
  {
    if (event.IsHandled()) { return; }
    // if listener being pointed to no longer exists, remove it
    if (!(*it))
    {
      it = subscribers.erase(it);
      continue;
    }

    (*it)->HandleEvent(static_cast<Event*>(&event));
    ++it;
  }
}

template <typename EventType>
void EventManager::Dispatch(EventType&& event)
{
  std::vector<IEventListener*>& subscribers{ m_subscribers[typeid(EventType)].m_listeners };
  for (std::vector<IEventListener*>::iterator it{ subscribers.begin() }; it != subscribers.end();)
  {
    if (event.IsHandled()) { return; }

    // if listener being pointed to no longer exists, remove it
    if (!(*it))
    {
      it = subscribers.erase(it);
      continue;
    }

    (*it)->HandleEvent(static_cast<Event*>(&event));
    ++it;
  }
}