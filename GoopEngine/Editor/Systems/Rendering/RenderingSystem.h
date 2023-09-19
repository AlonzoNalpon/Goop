#ifndef RENDERING_SYSTEM_H
#define RENDERING_SYSTEM_H
#include <iostream>
#include <ECS/System/System.h>										// System class to inherit
#include <ECS/EntityComponentSystem.h>						// the ECS system
#include <Systems/Rendering/RendererComponent.h>	// the component related to this system
namespace Systems {
  class RenderSystem : public GE::ECS::System
  {
	public:
		void Awake()
		{
			std::cout << "Rendering system awoken\n";
		}

		void Start()
		{
			std::cout << "Rendering system started\n";
		}

		void Update()
		{

			//for (auto& entt : m_entities)
			//{
			//	using RenderComponent = Components::Renderer;
			//	GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
			//
			//	RenderComponent* txt = ecs.GetComponent<RenderComponent>(entt);
			//	if (txt == nullptr)
			//	{
			//		continue;
			//	}
			//
			//}
		}

		void OnDestroyed()
		{
			std::cout << "Rendering system destroyed\n";
		}
  };
}

#endif
