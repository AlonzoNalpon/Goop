﻿/*!*********************************************************************
\file   MineWormScratches.cs
\date   04-February-2024
\brief  Script to play MineWorm scratch sound

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Audio.MineWorm
{
  public class MineWormScratches : IAnimationEvent
  {
    public int startRange, endRange;
    public void PlayEvent(uint entity)
    {
      Utils.PlayRandomSound(startRange, endRange, entity, 0.75f);
    }
  }
}
