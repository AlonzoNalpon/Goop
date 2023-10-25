#include <pch.h>
#include "SystemManager.h"
#include <DebugTools/ErrorLogger/ErrorLogger.h>
#include "../../FrameRateController/FrameRateController.h"
#include <sstream>

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

std::unordered_map<const char*, ComponentSignature>& GE::ECS::SystemManager::GetSystemSignatures()
{
	return m_signatures;
}

std::map<int, const char*>& GE::ECS::SystemManager::GetSystemIndexes()
{
	return m_indexToSystem;
}

void SystemManager::EntityDestroyed(const Entity& entity)
{
	for (auto& system : m_systems)
	{
		system.second->GetEntities().erase(entity);
		system.second->GetInActiveEntities().erase(entity);
		system.second->GetAllEntities().erase(entity);
	}
}

void SystemManager::EntitySignatureChanged(Entity& entity, const ComponentSignature& signature, bool isActive)
{
	for (auto& system : m_systems)
	{
		ComponentSignature& sySig{ m_signatures[system.first] };

		// checks if the entity's has a component used by the system
		if ((signature & sySig) != sySig)
		{
			// Entity does not have component to run this system
			// remove it from system
			// std::set.erase() does nothing if object does not exist
			// no need for error handling
			system.second->GetEntities().erase(entity);
			system.second->GetInActiveEntities().erase(entity);
			system.second->GetAllEntities().erase(entity);
			std::stringstream ss;
			ss << "Entity ID " << entity << " does not match " << system.first << " siganture. Removed from entity list";
			GE::Debug::ErrorLogger::GetInstance().LogMessage<SystemManager>(ss.str());
		}
		else
		{			
			system.second->GetAllEntities().insert(entity);
			if (isActive)
			{
				system.second->GetEntities().insert(entity);
			}
			else
			{
				system.second->GetInActiveEntities().insert(entity);
			}
		}
	}
}

void GE::ECS::SystemManager::EntityActiveStateChanged(Entity& entity, bool newState)
{
	for (auto& system : m_systems)
	{
		system.second->EntityActiveStateChanged(entity, newState);
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

	GE::FPS::FrameRateController& fpsC = GE::FPS::FrameRateController::GetInstance();
	fpsC.StartSystemTimer();
	// Use index to system here as index contains systems in the order which it should update in
	for (auto& system : m_indexToSystem)
	{
		auto& systemName{ system.second };
		m_systems[systemName]->Update();
	}
	fpsC.EndSystemTimer("System Update");

	int steps = fpsC.GetSteps();
	if (steps > 0)
	{
		for (auto& system : m_indexToSystem)
		{
			auto& systemName{ system.second };
			for (int i{}; i < steps; ++i)
			{
				m_systems[systemName]->FixedUpdate();
			}
		}
	}
	fpsC.EndSystemTimer("System Fixed Update");

	for (auto& system : m_indexToSystem)
	{
		auto& systemName{ system.second };
		m_systems[systemName]->LateUpdate();
	}
	fpsC.EndSystemTimer("System Late Update");
}

void SystemManager::UpdateSystemsFixed()
{
	for (auto& system : m_systems)
	{
		system.second->FixedUpdate();
	}
}