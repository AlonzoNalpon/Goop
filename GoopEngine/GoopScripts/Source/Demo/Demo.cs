/*!*********************************************************************
\file   Demo.cs
\author chengen.lau\@digipen.edu
\date   27-January-2024
\brief  Script used for the presentation demo

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using GoopScripts.Gameplay;

namespace GoopScripts.Demo
{
  internal class Demo : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      //Utils.PlaySoundF("SFX_BodyFall3", 1.0f, Utils.ChannelType.SFX, false);
      //Utils.SetTextComponent((int)entity, "HELLO");
    }
  }
}
