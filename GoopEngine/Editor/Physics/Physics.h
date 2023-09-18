#include <pch.h>

namespace GE
{
	namespace Physics
	{
		class PhysSys : public GE::ECS::System
		{
		public:
			void Awake();
			void Update();

		private:
			EntityComponentSystem* m_ecs;
		};
	}
}