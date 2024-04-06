/*!*********************************************************************
\file   ParticleSystem.h
\author w.chinkitbryan\@digipen.edu
\date   23-January-2024
\brief
  Definition of the particle system class. Defines an update function
  to run the emitters of entities in the system to create particle
  effects.
  
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <ECS/System/System.h>

namespace GE::Systems
{
  class ParticleSystem : public GE::ECS::System
  {
    /*!*********************************************************************
    \brief
      Iterates through all entities in the system to update the particles
      in the emitter based on their lifetime and values.
    ************************************************************************/
    void Update();
  };
}
