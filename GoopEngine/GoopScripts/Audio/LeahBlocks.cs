using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Gameplay
{
  internal class LeahBlocks : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("SFX_ShieldUp", 1.0f, Utils.ChannelType.SFX, false);
    }
  }
}
