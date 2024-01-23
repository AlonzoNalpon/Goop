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
    bool m_hasGravity{false};
    GE::Math::dVec2 m_minForce;
    GE::Math::dVec2 m_maxForce;
    float m_minDrag;
    float m_maxDrag;
    float m_minLifeTime;
    float m_maxLifeTime;
    int m_particlesPerSecond;

    std::map<GE::ECS::Entity, float> m_lifeTimes;
  };
}
