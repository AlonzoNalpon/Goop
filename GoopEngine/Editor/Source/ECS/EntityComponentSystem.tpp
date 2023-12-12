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
	m_systemManager->EntitySignatureChanged(entity, signature, m_entityManager->IsActiveEntity(entity));
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

	// Update all systems to remove entity if it does not have
	// the components required to be used by the system 
	m_systemManager->EntitySignatureChanged(entity, signature, m_entityManager->IsActiveEntity(entity));
}

template <typename T>
T* EntityComponentSystem::GetComponent(const Entity& entity)
{
	return m_componentManager->GetComponent<T>(entity);
}

template <typename T>
bool EntityComponentSystem::HasComponent(const Entity& entity) const
{
	return m_componentManager->HasComponent<T>(entity);
}

template <typename T>
ComponentType EntityComponentSystem::GetComponentSignature() const
{
	return m_componentManager->GetComponentSignature<T>();
}

template <typename T>
T* EntityComponentSystem::GetSystem()
{
	return m_systemManager->GetSystem<T>();
}

template <typename T>
T* EntityComponentSystem::RegisterSystem()
{
	return m_systemManager->RegisterSystem<T>();
}

template <typename T>
bool EntityComponentSystem::RemoveSystem()
{
	return m_systemManager->RemoveSystem<T>();
}

template <typename C, typename S>
void EntityComponentSystem::RegisterComponentToSystem()
{
	ComponentSignature sysSig = m_systemManager->GetSignature<S>();
	sysSig.set(m_componentManager->GetComponentSignature<C>(), true);
	m_systemManager->SetSignature<S>(sysSig);
}

template <typename C, typename S>
void EntityComponentSystem::UnregisterComponentFromSystem()
{
	ComponentSignature sysSig = m_systemManager->GetSignature<S>();
	sysSig.set(m_componentManager->GetComponentSignature<C>(), false);
	m_systemManager->SetSignature<S>(sysSig);
}

template <typename T>
void EntityComponentSystem::SetSystemSignature(const ComponentSignature& signature)
{
	m_systemManager->SetSignature<T>(signature);
}

template <typename T>
ComponentSignature EntityComponentSystem::GetSystemSignature()
{
	return m_systemManager->GetSignature<T>();
}

template <typename T>
bool EntityComponentSystem::RegisterEntityToSystem(Entity& entity)
{
	ComponentSignature sig = m_entityManager->GetComponentSignature(entity);
	return m_systemManager->RegisterEntityToSystem<T>(entity, sig, m_entityManager->IsActiveEntity(entity));
}

template <typename T>
bool EntityComponentSystem::UnregisterEntityFromSystem(Entity& entity)
{
	return m_systemManager->UnregisterEntityFromSystem<T>(entity);
}