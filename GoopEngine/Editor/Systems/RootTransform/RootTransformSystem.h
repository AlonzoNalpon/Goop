#pragma once
#include <pch.h>

namespace GE::Systems
{
	class RootTransformSystem : public GE::ECS::System
	{
		void Update();

		void Propergate(GE::ECS::Entity& entity, const Math::dMat3& parentWorldTrans);
	};
}
