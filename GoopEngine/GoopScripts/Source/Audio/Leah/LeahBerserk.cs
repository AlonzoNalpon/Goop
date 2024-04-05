/*!*********************************************************************
\file   LeahBerserk.cs
\date   06-April-2024
\brief  Script to play SFX for Leah's Berserk card

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Audio.Leah
{
  public class LeahBerserk : IAnimationEvent
  {
    public string soundName;
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF(soundName, 0.7f, Utils.ChannelType.SFX, false);
    }
  }
}
