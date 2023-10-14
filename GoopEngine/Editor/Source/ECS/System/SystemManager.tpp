template <typename T>
T* SystemManager::GetSystem()
{	
	const char* systemName = typeid(T).name();

	if (m_systems.find(systemName) != m_systems.end())
	{
		return static_cast<T*>(m_systems[systemName]);	
	}
	
	std::stringstream ss;
	ss << "Fetching system " << systemName << " while it does not exist. No action taken";
	GE::Debug::ErrorLogger::GetInstance().LogMessage<SystemManager>(ss.str(), false);
	return nullptr;
}

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

	if (m_systems.find(systemName) == m_systems.end() ||
		m_signatures.find(systemName) == m_signatures.end())
	{
		// Removing a system that does not exist
		std::stringstream ss;
		ss << "Removing system " << systemName << " while it does not exist. No action taken";
		GE::Debug::ErrorLogger::GetInstance().LogMessage<SystemManager>(ss.str(), false);
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
bool SystemManager::RegisterEntityToSystem(Entity& entity, ComponentSignature& signature, bool activeState)
{
	const char* systemName = typeid(T).name();
	if (m_systems.find(systemName) == m_systems.end())
	{
		// System does not exist
		std::stringstream ss;
		ss << "Failed to register entity ID " << entity << ". System " << systemName << " does not exist.";
		GE::Debug::ErrorLogger::GetInstance().LogError<SystemManager>(ss.str());
		return false;
	}

	ComponentSignature cmpSig{ GetSignature<T>() };
	// checks if the entity's has a component used by the system
	if ((signature & cmpSig) != cmpSig)
	{
		// Entity does not match system signature
		std::stringstream ss;
		ss << "Failed to register entity ID " << entity << ". Entity does not match " << systemName << " component signature.";
		GE::Debug::ErrorLogger::GetInstance().LogError<SystemManager>(ss.str());
		return false;
	}

	if (activeState)
	{
		m_systems[systemName]->GetEntities().insert(entity);
	}
	else
	{
		m_systems[systemName]->GetInActiveEntities().insert(entity);
	}
	m_systems[systemName]->GetAllEntities().insert(entity);
	return true;
}

template <typename T>
bool SystemManager::UnregisterEntityFromSystem(Entity& entity)
{
	const char* systemName = typeid(T).name();
	if (m_systems.find(systemName) == m_systems.end())
	{
		// System does not exist
		std::stringstream ss;
		ss << "Removing entity from system  " << systemName << " while it does not exist. No action taken";
		GE::Debug::ErrorLogger::GetInstance().LogMessage<SystemManager>(ss.str(), false);
		return false;
	}

	// Remove entity from both list
	m_systems[systemName]->GetEntities().erase(entity);
	m_systems[systemName]->GetInActiveEntities().erase(entity);
	m_systems[systemName]->GetAllEntities().erase(entity);
	return true;
}
}