#include <pch.h>
#include <Systems/AnimEvent/AnimEventsSystem.h>
namespace GE::Systems
{
  void AnimEventsSystem::Update()
  {
    for (GE::ECS::Entity entity : GetUpdatableEntities())
    {
      Component::AnimEvents* animEvents{ m_ecs->GetComponent<Component::AnimEvents>(entity) };
      Component::SpriteAnim* spriteAnim{ m_ecs->GetComponent<Component::SpriteAnim>(entity) };
      if (!animEvents || spriteAnim)
        continue;

      spriteAnim->currFrame;
    }
  }
}
