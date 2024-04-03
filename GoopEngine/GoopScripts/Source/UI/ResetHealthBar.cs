using GoopScripts.Mono;
using GoopScripts.UI;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.UI
{
  internal class ResetHealthBar : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      GoopScripts.Gameplay.Stats player = (GoopScripts.Gameplay.Stats)Utils.GetScript("Player", "Stats");
      player.m_healthBar.Reset();
    }
  }
}
