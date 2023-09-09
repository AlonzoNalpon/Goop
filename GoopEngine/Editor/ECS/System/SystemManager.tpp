SystemManager::~SystemManager()
{
	for (auto& system : m_systems)
	{
		system.second->OnDestroyed();
		delete system.second;
		system.second = nullptr;
	}
}

template <typename T>
T* SystemManager::RegisterSystem()
{
	const char* systemName = typeid(T).name();

	if (m_systems.find(systemName) != m_systems.end())
	{
		// System exist unable to register
		return nullptr;
	}

	T* system = new T();
	system->Awake();
	m_systems[systemName] = system;
	m_uninitializedSystems.push(systemName);

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

	// Erase system from map
	m_systems.erase(systemName);
	m_signatures.erase(systemName);
	return true;
}

template <typename T>
bool SystemManager::SetSignature(const ComponentSignature& signature)
{
	const char* systemName = typeid(T).name();

	if (m_systems.find(systemName) == m_systems.end())
	{
		// System signature set before system exist
		return false;
	}

	m_signatures[systemName] = signature;
	return true;
}

void SystemManager::EntityDestroyed(const Entity& entity)
{
	for (auto& system : m_systems)
	{
		system.second->GetEntities().erase(entity);
	}
}

void SystemManager::EntitySignatureChanged(Entity& entity, const ComponentSignature& signature)
{
	for (auto& system : m_systems)
	{
		ComponentSignature& cmpSig{ m_signatures[system.first] };

		// checks if the entity's has a component used by the system
		if ((signature & cmpSig) != cmpSig)
		{
			// Entity does not have component to run this system
			// remove it from system
			// std::set.erase() does nothing if object does not exist
			// no need for error handling
			system.second->GetEntities().erase(entity);
		}
	}
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

void SystemManager::UpdateSystems()
{
	// Initialize all systems
	while (m_uninitializedSystems.size() > 0)
	{
		m_systems[m_uninitializedSystems.front()]->Start();
		m_uninitializedSystems.pop();
	}

	for (auto& system : m_systems)
	{
		system.second->Update();
		system.second->LateUpdate();
	}
}

void SystemManager::UpdateSystemsFixed()
{
	for (auto& system : m_systems)
	{
		system.second->FixedUpdate();
	}
}