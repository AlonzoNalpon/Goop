/*!*********************************************************************
\file   MoleRatBlocks.cs
\date   04-February-2024
\brief  Script to play mole rat block sound

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
  internal class MoleRatBlocks : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("SFX_Block2", 1.0f, Utils.ChannelType.SFX, false);
    }
  }
}
