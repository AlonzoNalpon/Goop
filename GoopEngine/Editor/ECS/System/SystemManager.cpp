#include "SystemManager.h"

using namespace GE::ECS;

SystemManager::~SystemManager()
{
	for (auto& system : m_systems)
	{
		system.second->OnDestroyed();
		delete system.second;
		system.second = nullptr;
	}
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

void SystemManager::UpdateSystems()
{
	// Initialize all systems
	while (m_uninitializedSystems.size() > 0)
	{
		// Somehow system to initialize doesn't exist
		if (m_systems.find(m_uninitializedSystems.front()) == m_systems.end())
		{
			m_uninitializedSystems.pop();
			continue;
		}

		m_systems[m_uninitializedSystems.front()]->Start();
		m_uninitializedSystems.pop();
	}

	for (auto system : m_indexToSystem)
	{
		auto& systemName{ system.second };
		m_systems[systemName]->Update();
		m_systems[systemName]->LateUpdate();
	}
}

void SystemManager::UpdateSystemsFixed()
{
	for (auto& system : m_systems)
	{
		system.second->FixedUpdate();
	}
}