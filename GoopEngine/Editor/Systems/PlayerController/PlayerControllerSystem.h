#pragma once
#include <pch.h>

using vec2 = GE::Math::dVec2;

namespace GE
{
	namespace Systems
	{
		class PlayerControllerSystem : public GE::ECS::System
		{
		public:
			void Awake();

			void Update();

			vec2 Tweening(vec2 start, vec2 end, double normalisedTime);
		};
	}
}