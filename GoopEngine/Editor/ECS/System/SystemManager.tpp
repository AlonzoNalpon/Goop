template <typename T>
T* SystemManager::RegisterSystem()
{
	const char* systemName = typeid(T).name();

	if (m_systems.find(systemName) != m_systems.end())
	{
		// System exist, return a pointer to it
		return static_cast<T*>(m_systems[systemName]);
	}

	T* system = new T();
	system->Awake();
	m_systems[systemName] = system;
	m_uninitializedSystems.push(systemName);

	m_systemToIndex[systemName] = m_systemIndex;
	m_indexToSystem[m_systemIndex] = systemName;
	++m_systemIndex;

	return system;
}

template<typename T>
bool SystemManager::RemoveSystem()
{
	const char* systemName = typeid(T).name();

	if (m_systems.find(systemName) == m_systems.end())
	{
		// Removing a system that does not exist
		return false;
	}
	if (m_signatures.find(systemName) == m_signatures.end())
	{
		// Somehow the signature for this system does not exist
		return false;
	}

	// Delete allocated system pointer
	System* system = m_systems[systemName];
	system->OnDestroyed();
	delete system;
	system = nullptr;

	int idxToRemove = m_systemToIndex[systemName];
	m_systemToIndex.erase(systemName);
	m_indexToSystem.erase(idxToRemove);

	// Erase system from map
	m_systems.erase(systemName);
	m_signatures.erase(systemName);
	return true;
}

template <typename T>
void SystemManager::SetSignature(const ComponentSignature& signature)
{
	const char* systemName = typeid(T).name();

	if (m_systems.find(systemName) == m_systems.end())
	{
		RegisterSystem<T>();
	}

	m_signatures[systemName] = signature;
}

template <typename T>
ComponentSignature SystemManager::GetSignature()
{
	const char* systemName = typeid(T).name();

	if (m_systems.find(systemName) == m_systems.end())
	{
		RegisterSystem<T>();
	}

	return m_signatures[systemName];
}

template <typename T>
bool SystemManager::RegisterEntityToSystem(Entity& entity)
{
	const char* systemName = typeid(T).name();
	if (m_systems.find(systemName) == m_systems.end())
	{
		// System does not exist
		return false;
	}

	m_systems[systemName]->GetEntities().insert(entity);
	return true;
}

template <typename T>
bool SystemManager::UnregisterEntityFromSystem(Entity& entity)
{
	const char* systemName = typeid(T).name();
	if (m_systems.find(systemName) == m_systems.end())
	{
		// System does not exist
		return false;
	}

	m_systems[systemName]->GetEntities().erase(entity);
	return true;
}
