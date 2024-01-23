#include <pch.h>
#include <Systems/ParitcleSystem/ParticleSystem.h>

void GE::Systems::ParticleSystem::Update()
{
  static auto& frc = GE::FPS::FrameRateController::GetInstance();
  float dt = static_cast<float>(frc.GetDeltaTime());

  for (const GE::ECS::Entity& entity : GetUpdatableEntities())
  {
    GE::Component::Emitter* em = m_ecs->GetComponent<GE::Component::Emitter>(entity);
    GE::Component::Sprite* sp = m_ecs->GetComponent<GE::Component::Sprite>(entity);
    if (em && sp)
    {
      if (!em->m_hasPlayed && em->m_playOnStart)
      {
        em->m_hasPlayed = true;
        em->m_playing = true;

        em->m_particleTime = em->m_particlesPerMin / 60.f;
      }
      
      // Destroy all particles that life out their lifetime
      for (const auto& [particle, lifetime] : em->m_lifeTimes)
      {
        if (lifetime < 0)
        {
          m_ecs->DestroyEntity(*const_cast<GE::ECS::Entity*>(&particle));
          // Can't remove element in loop operation so cannot remove from map
        }
      }
      // Remove all particles from the container when life time is over
      std::erase_if(em->m_lifeTimes, 
        [](const decltype(*em->m_lifeTimes.begin())& lifetime) 
        { 
          return lifetime.second < 0; 
        });

      if (em->m_playing)
      {
        if (em->m_currTime > em->m_particleTime)
        {
          em->m_currTime -= em->m_particleTime;

          // Create new particle entity
          GE::ECS::Entity particle = m_ecs->CreateEntity();

          // Parent to emitter
          m_ecs->SetParentEntity(entity);

          // Add all components
          GE::Component::Transform trans{};
          m_ecs->AddComponent(particle, trans);

          // Use emitter's sprite for particle
          m_ecs->AddComponent(particle, *sp);

          // Add physics
          GE::Component::Velocity vel{};
          vel.m_dragForce = GE::GoopUtils::Lerp(em->m_minDrag, em->m_maxDrag, GE::GoopUtils::RandomValue(0.f, 1.f));
          // Forced to each component manually cuz double Vec * float is not supported
          GE::Math::dVec3 force;
          float forceRand = GE::GoopUtils::RandomValue(0.f, 1.f);
          force.x = GE::GoopUtils::Lerp(em->m_minForce.x, em->m_maxForce.x, forceRand);
          force.y = GE::GoopUtils::Lerp(em->m_minForce.y, em->m_maxForce.y, forceRand);
          force.z = GE::GoopUtils::Lerp(em->m_minForce.z, em->m_maxForce.z, forceRand);
          vel.AddForce(force, em->m_maxLifeTime, true);
          vel.m_gravity = em->m_gravity;
          m_ecs->AddComponent(particle, vel);

          em->m_lifeTimes[particle] = GE::GoopUtils::Lerp(0.f, em->m_maxLifeTime, GE::GoopUtils::RandomValue(0.f, 1.f));
        }
        
        // Update all timers
        em->m_currTime += dt;
        std::for_each(em->m_lifeTimes.begin(), em->m_lifeTimes.end(), 
          [dt](decltype(*em->m_lifeTimes.begin())& lifetime) 
          { 
            lifetime.second -= dt; 
          });
      }
    }
  }
}
