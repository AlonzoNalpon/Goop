EntityComponentSystem::~EntityComponentSystem()
{
	delete m_componentManager;
	m_componentManager = nullptr;

	delete m_entityManager;
	m_entityManager = nullptr;

	delete m_systemManager;
	m_systemManager = nullptr;
}

void EntityComponentSystem::Init()
{
	m_componentManager = new ComponentManager();
	m_entityManager = new EntityManager(4092);
	m_systemManager = new SystemManager();
}

Entity EntityComponentSystem::CreateEntity()
{
	return m_entityManager->CreateEntity();
}

void EntityComponentSystem::DestroyEntity(Entity& entity)
{
	m_entityManager->DestroyEntity(entity);
	m_componentManager->EntityDestroyed(entity);
	m_systemManager->EntityDestroyed(entity);
}

template <typename T>
void EntityComponentSystem::RegisterComponent()
{
	m_componentManager->RegisterComponent<T>();
}

template <typename T>
void EntityComponentSystem::AddComponent(Entity& entity, T component)
{
	m_componentManager->AddComponent(entity, component);

	// Get the current entity's component bitflag
	ComponentSignature signature = m_entityManager->GetComponentSignature(entity);
	// Adds this new component's bitflag to new signature
	signature.set(m_componentManager->GetComponentSignature<T>(), true);
	// Update entity's component signature
	m_entityManager->SetComponentSignature(entity, signature);
}

template <typename T>
void EntityComponentSystem::RemoveComponent(Entity& entity)
{
	m_componentManager->RemoveComponent<T>(entity);

	// Get the current entity's component bitflag
	ComponentSignature signature = m_entityManager->GetComponentSignature(entity);
	// Remove this new component bitflag from signature
	signature.set(m_componentManager->GetComponentSignature<T>(), false);
	// Update entity's component signature
	m_entityManager->SetComponentSignature(entity, signature);
}

template <typename T>
T* EntityComponentSystem::GetComponent(const Entity& entity)
{
	return m_componentManager->GetComponent<T>(entity);
}

template <typename T>
ComponentType EntityComponentSystem::GetComponentSignature()
{
	return m_componentManager->GetComponentSignature<T>();
}

template <typename T>
T* EntityComponentSystem::RegisterSystem()
{
	return m_systemManager->RegisterSystem<T>();
}

template <typename T>
void EntityComponentSystem::RemoveSystem()
{
	m_systemManager->RemoveSystem<T>();
}

template <typename T>
void EntityComponentSystem::SetSystemSignature(const ComponentSignature& signature)
{
	m_systemManager->SetSignature<T>(signature);
}

void EntityComponentSystem::UpdateSystems()
{
	m_systemManager->UpdateSystems();
}