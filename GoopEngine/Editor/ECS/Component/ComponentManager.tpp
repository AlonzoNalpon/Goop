ComponentManager::~ComponentManager()
{
	for (auto& cmpArr : m_componentArrays)
	{
		if (cmpArr.second == nullptr)
		{
			continue;
		}
		delete[] cmpArr.second;
		cmpArr.second = nullptr;
	}
}

template <typename T>
ComponentType ComponentManager::GetComponentSignature()
{
	const char* componentName = typeid(T).name();

	if (m_componentTypes.find(componentName) == m_componentTypes.end())
	{
		RegisterComponent<T>();
	}

	return m_componentTypes[componentName];
}

template <typename T>
void ComponentManager::RegisterComponent()
{
	const char* componentName = typeid(T).name();

	m_componentTypes[componentName] = m_nextComponentType++;
	m_componentArrays[componentName] = new ComponentArray<T>();
}

template <typename T>
void ComponentManager::AddComponent(Entity& entity, const T& component)
{
	// If new instance of T, make a new component array of type T
	if (m_componentTypes.find(typeid(T).name()) == m_componentTypes.end())
	{
		RegisterComponent<T>();
	}

	GetComponentArray<T>()->Insert(entity, component);
}

template <typename T>
void ComponentManager::RemoveComponent(Entity& entity)
{
	GetComponentArray<T>()->Remove(entity);
}

template <typename T>
T* ComponentManager::GetComponent(const Entity& entity)
{
	return GetComponentArray<T>()->GetData(entity);
}

void ComponentManager::EntityDestroyed(Entity& entity)
{
	for (auto& cmpArr : m_componentArrays)
	{
		cmpArr.second->EntityDestroyed(entity);
	}
}

template <typename T>
ComponentArray<T>* ComponentManager::GetComponentArray()
{
	return static_cast<ComponentArray<T>*>(m_componentArrays[typeid(T).name()]);
}