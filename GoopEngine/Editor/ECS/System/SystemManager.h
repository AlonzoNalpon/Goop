#pragma once
#include <unordered_map>
#include <queue>
#include "../Entity/Entity.h"
#include "System.h"
#include <iostream>
namespace GE
{
	namespace ECS
	{
		class SystemManager
		{
		public:
			SystemManager() = default;
			~SystemManager();

			template <typename T>
			T* RegisterSystem();
			template <typename T>
			bool RemoveSystem();
			template <typename T>
			bool SetSignature(const ComponentSignature& signature);
			void EntityDestroyed(const Entity& entity);
			void EntitySignatureChanged(Entity& entity, const ComponentSignature& signature);

			template <typename T>
			bool RegisterEntityToSystem(Entity& entity);
			template <typename T>
			bool UnregisterEntityFromSystem(Entity& entity);

			void UpdateSystems();
			// Framerate controller to call this at fixed time intervals
			// usually reserved for consistent physics calculations
			void UpdateSystemsFixed();
		private:
			// This is a map of unique signatures each system has
			// a system signature consist of the signatures of all the
			// components used in the system
			std::unordered_map<const char*, ComponentSignature> m_signatures;
			std::unordered_map<const char*, System*> m_systems;
			std::queue<const char*> m_uninitializedSystems;
		};

//#include "SystemManager.tpp"
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

			m_systems[systemName]->OnDestroyed();
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
	}
}
