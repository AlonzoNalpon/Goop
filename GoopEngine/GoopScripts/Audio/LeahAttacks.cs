using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Gameplay
{
  internal class LeahAttacks : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("SFX_Beam3", 1.0f, Utils.ChannelType.SFX, false);
    }
  }
}
