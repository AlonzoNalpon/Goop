/*!*********************************************************************
\file   ParticleSystem.cpp
\author w.chinkitbryan\@digipen.edu
\date   23-January-2024
\brief
  Definition of the particle system class. Defines an update function
  to run the emitters of entities in the system to create particle
  effects.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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
      // Turn on rendering to be copied to child entity
      sp->m_shouldRender = true;
      if (!em->m_hasPlayed && em->m_playOnStart)
      {
        em->m_hasPlayed = true;
        em->m_playing = true;
      }
      em->m_particleTime = 60.f / em->m_particlesPerMin;
      
      // Destroy all particles that live out their lifetime
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
        while (em->m_currTime > em->m_particleTime)
        {
          if (em->m_emitterHasLifeCount && em->m_particleLifeCount < 1)
          {
            break;
          }

          em->m_currTime -= em->m_particleTime;

          // Create new particle entity
          GE::ECS::Entity particle = m_ecs->CreateEntity();

          // Parent to emitter
          m_ecs->SetParentEntity(particle, entity);
          m_ecs->AddChildEntity(entity, particle);

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
          force.x = GE::GoopUtils::Lerp(em->m_minVel.x, em->m_maxVel.x, forceRand);
          forceRand = GE::GoopUtils::RandomValue(0.f, 1.f);
          force.y = GE::GoopUtils::Lerp(em->m_minVel.y, em->m_maxVel.y, forceRand);
          forceRand = GE::GoopUtils::RandomValue(0.f, 1.f);
          force.z = GE::GoopUtils::Lerp(em->m_minVel.z, em->m_maxVel.z, forceRand);
          vel.m_vel += force;
          vel.m_gravity = em->m_gravity;
          m_ecs->AddComponent(particle, vel);

          em->m_lifeTimes[particle] = GE::GoopUtils::Lerp(em->m_minLifeTime, em->m_maxLifeTime, GE::GoopUtils::RandomValue(0.f, 1.f));
          
          // No need to check if should reduce, if it shouldn't then this
          // will be uninitialized but unused
          --em->m_particleLifeCount;
        }
        
        // Update all timers
        em->m_currTime += dt;
        std::for_each(em->m_lifeTimes.begin(), em->m_lifeTimes.end(), 
          [dt](decltype(*em->m_lifeTimes.begin())& lifetime) 
          { 
            lifetime.second -= dt;
          });
      }
    
      sp->m_shouldRender = false;
    }
  }
}