#pragma once
#include <pch.h>
#include <Component/Transform.h>

using vec2 = GE::Math::dVec2;

namespace GE
{
	namespace Systems
	{
		class PlayerControllerSystem : public GE::ECS::System, public GE::Events::EventListener
		{
		public:
			void Awake();
			void Start();
			void Update();
			void HandleEvent(Events::Event const* event) override;

			vec2 Tweening(vec2 start, vec2 end, double normalisedTime);
		};

	}
}




