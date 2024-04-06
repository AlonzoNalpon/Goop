/*!*********************************************************************
\file     ResetHealthBar.cs
\author   chengen.lau\@digipen.edu
\date     02-April-2024
\briefq   Script to trigger the resetting of the healthbar after the
          tween animation is complete.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using GoopScripts.UI;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.UI
{
  public class ResetHealthBar : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      GoopScripts.Gameplay.Stats player = (GoopScripts.Gameplay.Stats)Utils.GetScript("Player", "Stats");
      player.m_healthBar.Reset();
    }
  }
}
