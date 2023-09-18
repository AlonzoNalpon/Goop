#include "ComponentManager.h"

using namespace GE::ECS;

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

void ComponentManager::EntityDestroyed(Entity& entity)
{
	for (auto& cmpArr : m_componentArrays)
	{
		cmpArr.second->EntityDestroyed(entity);
	}
}