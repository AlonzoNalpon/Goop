


template <typename T>
void ComponentArray<T>::Insert(const Entity& entity, const T& component)
{
  T* block = reinterpret_cast<T*>(m_objAlloc.Allocate());
	std::cout << "COMP SIZE: " << sizeof(component) << "\n";
	new (block) T(component);
	m_components.emplace_back(block);
	size_t pushedIndex = m_components.size() - 1;

	m_entityToIndexMap[entity] = pushedIndex;
	m_indexToEntityMap[pushedIndex] = entity;
}

template <typename T>
void ComponentArray<T>::Remove(const Entity& entity)
{
	// Efficient removal while minimizing map keys changes
	// Moves the last item into the vector into the removed
	// entity's spot and then pop the last element
	if (m_entityToIndexMap.find(entity) == m_entityToIndexMap.end())
	{
		return;
	}

	size_t removedIdx = m_entityToIndexMap[entity];
	size_t indexOfLast = m_components.size() - 1;


	T* lastElement = 	m_components[removedIdx];
	std::cout << entity << ":: " << lastElement << "\n";
	m_objAlloc.Free(lastElement);

	m_components[removedIdx] = m_components[indexOfLast];

	// Update maps
	Entity entityOfLast = m_indexToEntityMap[indexOfLast];
	m_entityToIndexMap[entityOfLast] = removedIdx;
	m_indexToEntityMap[removedIdx] = entityOfLast;

	m_entityToIndexMap.erase(entity);
	m_indexToEntityMap.erase(indexOfLast);

	m_components.pop_back();

}

template <typename T>
T* ComponentArray<T>::GetData(const Entity& entity)
{
	std::stringstream ss;
	if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
	{
		size_t index = m_entityToIndexMap.at(entity);
		if (index <= m_components.size())
		{
			return &(*(m_components[index]));
		}
	}

	return nullptr;
}

template <typename T>
T* ComponentArray<T>::GetData(const Entity& entity) const
{
	// Use const_cast to call the non-const version
	return const_cast<T&>(const_cast<ComponentArray<T>*>(this)->GetData(entity));
}

template <typename T>
void ComponentArray<T>::EntityDestroyed(const Entity& entity)
{
	if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
	{
		Remove(entity);
	}
}