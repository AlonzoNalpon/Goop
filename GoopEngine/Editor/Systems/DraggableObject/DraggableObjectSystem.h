#pragma once
#include <pch.h>

namespace GE
{
	namespace Systems
	{
		class DraggableObjectSystem : public GE::ECS::System
		{
		public:
			void Update();
		};
	}
}