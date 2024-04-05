using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Audio.Leah
{
  public class LeahBerserk : IAnimationEvent
  {
    public string soundName;
    public void PlayEvent(uint entity)
    {
      Utils.PlaySoundF(soundName, 0.7f, Utils.ChannelType.SFX, false);
    }
  }
}
