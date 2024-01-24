#include <pch.h>
#include <Systems/ParitcleSystem/ParticleSystem.h>
#include <Component/Velocity.h>
#include <Component/Emitter.h>

void GE::Systems::ParticleSystem::Update()
{
  for (const GE::ECS::Entity& entity : GetUpdatableEntities())
  {
    GE::Component::Emitter* em = m_ecs->GetComponent<GE::Component::Emitter>(entity);
    if (em)
    {
      if (!em->m_hasPlayed && em->m_playOnStart)
      {
        em->m_hasPlayed = true;
        em->m_playing = true;


      }
    }
  }
}
