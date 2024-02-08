/*!*********************************************************************
\file   DisableEndTurn.cs
\author chengen.lau\@digipen.edu
\date   09-February-2024
\brief  Script to render the disabled end turn button during the
        resolution phase.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class DisableEndTurn : Entity
  {
    DisableEndTurn(uint entity) : base(entity) { }

    /*!*********************************************************************
     \brief
       Sets itself back to inactive upon the end of resolution phase
     \param deltaTime
       The delta time of the current frame
    ************************************************************************/
    public void OnUpdate(double deltaTime)
    {
      if (!GameManager.IsResolutionPhase())
      {
        Utils.SetIsActiveEntity(entityID, false);
      }
    }
  }
}
