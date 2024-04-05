/*!*********************************************************************
\file   LeahFlinches.cs
\date   04-February-2024
\brief  Script to play Leah flinch sound

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
  public class LeahHurt : IAnimationEvent
  {
    public int startRange, endRange;
    public void PlayEvent(uint entity)
    {
      Utils.SendString("Played flinch sound");
      Utils.PlayRandomSound(startRange, endRange, entity, 0.75f);
    }
  }
}
