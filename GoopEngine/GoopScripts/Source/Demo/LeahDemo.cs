/*!*********************************************************************
\file   LeahDemo.cs
\date   15-March-2024
\brief  Script used for demo

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using GoopScripts.Gameplay;

namespace GoopScripts.Gameplay
{
  internal class LeahDemo : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("SFX_BodyFall3", 1.0f, Utils.ChannelType.SFX, false);
      Utils.PlayAnimation("SS_Leah_Death", entity);
    }
  }
}
