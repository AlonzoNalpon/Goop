﻿/*!*********************************************************************
\file   MoleRatScreeches.cs
\date   04-February-2024
\brief  Script to play SFX for Mole Rat's Rage card

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Gameplay
{
  public class MoleRatScreeches : IAnimationEvent
  {
    public int startRange, endRange;
    public void PlayEvent(uint entity)
    {
      Utils.PlayRandomSound(startRange, endRange, entity, 0.75f);
    }
  }
}
