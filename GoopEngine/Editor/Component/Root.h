#pragma once

namespace GE::Component
{
	struct Root
	{
		// Literally does nothing.
		// Its just an identifier class so RootTransformSystem knows
		// which entity it should propergate the WorldTransform update from
	};
}
