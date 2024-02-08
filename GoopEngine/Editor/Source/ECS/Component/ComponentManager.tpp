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
	std::stringstream ss;
	ComponentArray<T>* compArr = GetComponentArray<T>();
	if (compArr)
	{		
		compArr->Remove(entity);
	}
	else
	{
		ss << "Cannot remove component of type " << typeid(T).name() << ". It does not exist";
	}
}

template <typename T>
T* ComponentManager::GetComponent(const Entity& entity)
{
	std::stringstream ss;
	ComponentArray<T>* compArr = GetComponentArray<T>();
	if (compArr)
	{		
		T* component = compArr->GetData(entity);
		if (component)
		{
			return component;		
		}
		else
		{
			ss << "Getting component of type " << typeid(T).name() << " from Entity ID " << entity << " that does not exist";
		}
	}
	else
	{
		ss << "Getting component of type " << typeid(T).name() << " that does not exist";
	}
	
	GE::Debug::ErrorLogger::GetInstance().LogMessage(ss.str(), false);
	return nullptr;
}

template <typename T>
bool ComponentManager::HasComponent(const Entity& entity)
{
	ComponentArray<T>* compArr = GetComponentArray<T>();
	if (compArr)
	{		
		T* component = compArr->GetData(entity);
		if (component)
		{
			return true;		
		}
	}

	return false;
}

template <typename T>
ComponentArray<T>* ComponentManager::GetComponentArray()
{
	char const* typeName = typeid(T).name();
	if (m_componentArrays.find(typeName) == m_componentArrays.end())
	{
		return nullptr;
	}

	return static_cast<ComponentArray<T>*>(m_componentArrays[typeName]);
}