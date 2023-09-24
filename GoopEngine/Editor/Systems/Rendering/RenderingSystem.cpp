#include <Systems/Rendering/RenderingSystem.h>
#include <Systems/Rendering/RendererComponent.h>	// the component related to this system

namespace GE::Systems
{

  void RenderSystem::Awake()
  {
  }

  void RenderSystem::Start()
  {
  }

  void RenderSystem::Update()
  {
    for (GE::ECS::Entity entity : m_entities)
    {
      
    }
  }

  void RenderSystem::OnDestroyed()
  {
  }

}