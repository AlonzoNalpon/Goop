#pragma once
#include <unordered_map>
#include "../Entity/Entity.h"
#include "System.h"

namespace GE
{
	namespace ECS
	{
		class SystemManager
		{
		public:
			template <typename T>
			T* RegisterSystem();

			template <typename T>
			void SetSignature(const ComponentSignature& signature);

			void EntityDestroyed(const Entity& entity);
			void EntitySignatureChanged(Entity& entity, const ComponentSignature& signature);
		private:
			std::unordered_map<const char*, ComponentSignature> m_signatures;
			std::unordered_map<const char*, System*> m_systems;
		};
	}
}
