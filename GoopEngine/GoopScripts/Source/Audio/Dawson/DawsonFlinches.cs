/*!*********************************************************************
\file   DawsonFlinches.cs
\date   04-February-2024
\brief  Script to play Dawson flinch sound

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Audio.Dawson
{
  internal class DawsonFlinches
  {
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("SFX_SlashSharp", 0.8f, Utils.ChannelType.SFX, false);
    }
  }
}
