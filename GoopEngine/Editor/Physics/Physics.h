#pragma once
//#include <pch.h>
#include "../ECS/EntityComponentSystem.h"
//#include "../ECS/System/System.h"
//#include "../FrameRateController/FrameRateController.h"

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
			//GE::ECS::EntityComponentSystem* m_ecs;
		};
	}
}