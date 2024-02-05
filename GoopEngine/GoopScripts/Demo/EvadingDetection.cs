using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GoopScripts.Mono;
using System.Threading.Tasks;

namespace GoopScripts.Gameplay
{
  internal class EvadingDetection : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("SFX_BodyFall3", 1.0f, Utils.ChannelType.SFX, false);
    }
  }
}
