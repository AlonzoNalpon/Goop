﻿/*!*********************************************************************
\file   LeahFlashBangs.cs
\date   04-February-2024
\brief  Script to play Leah flash bang SFX

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
  internal class LeahFlashBangs : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("woosh_scifi", 1.0f, Utils.ChannelType.SFX, false);
    }
  }
}
