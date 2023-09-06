#pragma once
#include "ComponentBase.h"

namespace GE
{
	namespace ECS
	{
		class SystemBase
		{
		private:
			using ComponentType = int;
			ComponentType m_id;

		protected:
			SystemBase();
			~SystemBase();

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
		};
	}
}
