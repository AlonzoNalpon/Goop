﻿/*!*********************************************************************
\file   LeahShoots.cs
\date   04-February-2024
\brief  Script to play Leah beam sound

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Gameplay
{
  public class LeahShoots : Entity, IAnimationEvent
  {
    public string[] variations = new string[6];

    public LeahShoots(uint entity) : base(entity) { }
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("SFX_Beam3", 1.0f, Utils.ChannelType.SFX, false);
    }
  }
}
