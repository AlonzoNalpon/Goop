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
			virtual ~System() = default;

			virtual void Awake();
			virtual void Start();

			virtual void Update();
			virtual void LateUpdate();
			virtual void FixedUpdate();

			virtual void OnDestroyed();

			virtual std::set<Entity>& GetEntities();

		protected:
			std::set<Entity> m_entities;
		};
	}
}
