#ifndef RENDERING_SYSTEM_H
#define RENDERING_SYSTEM_H
#include <pch.h>
#include <iostream>

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
