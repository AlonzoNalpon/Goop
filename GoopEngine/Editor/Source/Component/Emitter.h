/*!*********************************************************************
\file   Emitter.h
\author w.chinkitbryan\@digipen.edu
\date   25-January-2024
\brief  Component for creation of particles. Holds the details req1uired
        to emit a particle effect by creating objects with each their
        own values and lifetimes.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <ECS/Component/Component.h>
#include <Math/GEM.h>

namespace GE::Component
{
  struct Emitter
  {
    bool m_playOnStart{false};
    bool m_hasPlayed{false};
    bool m_playing{false};
    GE::Math::dVec3 m_gravity;
    GE::Math::dVec3 m_minVel;
    GE::Math::dVec3 m_maxVel;
    float m_minDrag;
    float m_maxDrag;
    float m_minLifeTime;
    float m_maxLifeTime;
    float m_currTime;
    float m_particleTime;
    int m_particlesPerMin;

    bool m_emitterHasLifeCount;
    int m_particleLifeCount;

    std::map<GE::ECS::Entity, float> m_lifeTimes;
  };
}
