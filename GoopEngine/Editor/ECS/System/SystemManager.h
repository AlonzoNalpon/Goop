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

#include "SystemManager.tpp"
	}
}
