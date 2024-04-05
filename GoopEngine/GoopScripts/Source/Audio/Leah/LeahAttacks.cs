/*!*********************************************************************
\file   LeahAttacks.cs
\date   04-February-2024
\brief  Script to play Leah attack sound

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
  public class LeahAttacks : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("SFX_Beam", 0.6f, Utils.ChannelType.SFX, false);
    }
  }
}
