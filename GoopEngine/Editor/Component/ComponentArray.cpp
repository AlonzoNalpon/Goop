#include "ComponentArray.h"

using namespace GE::ECS;

template <typename T>
void ComponentArray<T>::Insert(const Entity& entity, const T& component)
{
	m_components.emplace_back(component);
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

	size_t removedIdx = m_entityToIndexMap[entity];
	size_t indexOfLast = m_components.size() - 1;

	m_components[removedIdx] = m_components[indexOfLast];
	
	// Update maps
	Entity entityOfLast = m_indexToEntityMap[indexOfLast];
	m_entityToIndexMap[entityOfLast] = removedIdx;
	m_indexToEntityMap[removedIdx] = entityOfLast;

	// Remove last element
	m_components.pop_back();	
}

template <typename T>
T& ComponentArray<T>::GetData(const Entity& entity) const
{
	return m_components[m_entityToIndexMap[entity]];
}

template <typename T>
void ComponentArray<T>::EntityDestroyed(const Entity& entity)
{
	if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
	{
		Remove(entity);
	}
}
