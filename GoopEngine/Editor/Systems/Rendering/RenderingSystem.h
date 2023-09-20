#ifndef RENDERING_SYSTEM_H
#define RENDERING_SYSTEM_H
#include <iostream>
#include <ECS/System/System.h>										// System class to inherit
#include <ECS/EntityComponentSystem.h>						// the ECS system
#include <Systems/Rendering/RendererComponent.h>	// the component related to this system
namespace GE::Systems {
  class RenderSystem : public GE::ECS::System
  {
	public:
		void Awake();

		void Start();

		void Update();

		void OnDestroyed();
  };
}

#endif
