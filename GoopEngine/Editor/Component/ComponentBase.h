#pragma once

namespace GE
{
	namespace Component
	{
		class ComponentBase
		{
		private:
			using ComponentType = int;
			ComponentType m_id;
		protected:
			ComponentBase();
			~ComponentBase();

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
