#pragma once
#include <set>
#include "../Entity/Entity.h"

namespace GE
{
	namespace ECS
	{
		class System
		{
		public:
			System() = default;
			~System() = default;

		public:
			void Awake();
			void Start();

			void Update();
			void LateUpdate();
			void FixedUpdate();

			void OnDestroyed();

			inline std::set<Entity>& GetEntities();

		private:
			std::set<Entity> m_entities;
		};
	}
}
