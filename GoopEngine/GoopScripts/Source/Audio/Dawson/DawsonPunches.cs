/*!*********************************************************************
\file   DawsonPunches.cs
\date   04-February-2024
\brief  Script to play Dawson punch sound

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Audio.Dawson
{
  internal class DawsonPunches
  {
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("LightSaber", 0.7f, Utils.ChannelType.SFX, false);
    }
  }
}
