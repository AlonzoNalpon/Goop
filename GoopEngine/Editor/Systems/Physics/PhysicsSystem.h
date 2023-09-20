#pragma once
#include <pch.h>

namespace GE
{
	namespace Systems
	{
		class PhysicsSystem : public GE::ECS::System
		{
		public:
			void Update();
		};
	}
}