using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Gameplay
{
  internal class MoleRatBites : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("woosh2", 1.0f, Utils.ChannelType.SFX, false);
    }
  }
}
