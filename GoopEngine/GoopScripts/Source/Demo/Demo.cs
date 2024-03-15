using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using GoopScripts.Gameplay;

namespace GoopScripts.Demo
{
  internal class Demo : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF("SFX_BodyFall3", 1.0f, Utils.ChannelType.SFX, false);
      Utils.SetTextComponent((int)entity, "HELLO");
    }
  }
}
