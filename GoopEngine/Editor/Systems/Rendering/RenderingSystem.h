#ifndef RENDERING_SYSTEM_H
#define RENDERING_SYSTEM_H
#include <pch.h>
#include <iostream>

// The involved components:
#include <Component/Model.h>			// models (ALL MUST HAVE THIS)
#include <Component/Sprite.h>			// sprite (ALL MUST HAVE THIS)
#include <Component/SpriteAnim.h> // optional and will edit the sprite component
#include <Component/Transform.h>
namespace GE::Systems {
  class RenderSystem : public GE::ECS::System
  {
	public:

		void Update();

		void OnDestroyed();
  };
}

#endif
