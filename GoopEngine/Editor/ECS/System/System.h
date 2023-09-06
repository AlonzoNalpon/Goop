#pragma once
#include <set>
#include "../Entity/Entity.h"

namespace GE
{
	namespace ECS
	{
		class System
		{
		protected:
			System() = default;
			~System() = default;

		public:
			void Awake();
			void Start();

			void Update();
			void LateUpdate();
			void FixedUpdate();

			void OnDisable();
			void OnEnable();
			void OnDestroy();

			void SetActive(bool active);
			const bool& GetActive() const;

			std::set<Entity> m_entities;
		};
	}
}
