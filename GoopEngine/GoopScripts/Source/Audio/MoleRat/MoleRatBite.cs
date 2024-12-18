﻿/*!*********************************************************************
\file   MoleRatBites.cs
\date   04-February-2024
\brief  Script to play Mole Rat's attack SFX

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
  internal class MoleRatBite : IAnimationEvent
  {
    public int startRange, endRange;
    public void PlayEvent(uint entity)
    {
      startRange = 0;
      endRange = 0;
      Utils.PlayRandomSound(startRange, endRange, entity, 0.75f);
    }
  }
}
