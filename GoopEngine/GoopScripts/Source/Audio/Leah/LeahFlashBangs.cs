/*!*********************************************************************
\file   LeahFlashBangs.cs
\date   04-February-2024
\brief  Script to play Leah flash bang SFX

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
  internal class LeahFlashBangs : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("SFX_Leah_Flashbang", 0.8f, Utils.ChannelType.SFX, false);
    }
  }
}
