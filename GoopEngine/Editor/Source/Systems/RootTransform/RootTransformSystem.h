#pragma once
#include <Math/GEM.h>
#include <ECS/System/System.h>

namespace GE::Systems
{
	class RootTransformSystem : public GE::ECS::System
	{
		// Intentionally override this function as
		// it has different behaviour as it will handle
		// inactive entities as well
		void Update();

		void Propergate(GE::ECS::Entity& entity, const Math::dMat4& parentWorldTrans);
	};
}
