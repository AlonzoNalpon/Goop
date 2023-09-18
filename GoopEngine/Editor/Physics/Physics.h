#pragma once
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
			GE::ECS::EntityComponentSystem* m_ecs;
		};
	}
}