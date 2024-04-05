using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Audio.MineWorm
{
  public class MineWormFlinches : IAnimationEvent
  {
    public string soundName;
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF(soundName, 1.0f, Utils.ChannelType.SFX, false);
    }
  }
}
